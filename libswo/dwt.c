/*
 * This file is part of the libswo project.
 *
 * Copyright (C) 2016 Marc Schink <swo-dev@marcschink.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdbool.h>

#include "libswo-internal.h"

/* Event counter packet discriminator ID. */
#define EVTCNT_ID		0

/* Size of an event counter packet in bytes. */
#define EVTCNT_SIZE		2

/* Bitmask for the CPI flag of an event counter packet. */
#define EVTCNT_CPI_MASK		0x01

/* Bitmask for the Exc flag of an event counter packet. */
#define EVTCNT_EXC_MASK		0x02

/* Bitmask for the Sleep flag of an event counter packet. */
#define EVTCNT_SLEEP_MASK	0x04

/* Bitmask for the LSU flag of an event counter packet. */
#define EVTCNT_LSU_MASK		0x08

/* Bitmask for the Fold flag of an event counter packet. */
#define EVTCNT_FOLD_MASK	0x10

/* Bitmask for the Cyc flag of an event counter packet. */
#define EVTCNT_CYC_MASK		0x20

/* Exception trace packet discriminator ID. */
#define EXCTRACE_ID		1

/* Size of an exception trace packet in bytes. */
#define EXCTRACE_SIZE		3

/* Bitmask for the exception number of an exception trace packet. */
#define EXCTRACE_EX_MASK	0x01

/* Offset of the exception number of an exception trace packet. */
#define EXCTRACE_EX_OFFSET	8

/* Bitmask for the function of an exception trace packet. */
#define EXCTRACE_FN_MASK	0x30

/* Offset of the function of an exception trace packet. */
#define EXCTRACE_FN_OFFSET	4

/* Periodic PC sample packet discriminator ID. */
#define PC_SAMPLE_ID		2

/* Size of a periodic PC sleep packet in bytes. */
#define PC_SAMPLE_SLEEP_SIZE	2

/* Size of a periodic PC sample packet in bytes. */
#define PC_SAMPLE_SIZE		5

/* Bitmask for the comparator number of a data trace packet. */
#define CMPN_MASK		0x06

/* Offset of the comparator number of a data trace packet. */
#define CMPN_OFFSET		1

/* Bitmask for the WnR flag of a data trace data value packet. */
#define WNR_MASK		0x01

/* Bitmask for the data trace PC value packet header. */
#define PC_VALUE_HEADER_MASK	0x19

/* Data trace PC value packet header. */
#define PC_VALUE_HEADER		0x08

/* Size of a data trace PC value packet in bytes. */
#define PC_VALUE_SIZE		5

/* Bitmask for the data trace address offset packet header. */
#define ADDR_OFFSET_HEADER_MASK	0x19

/* Data trace address offset packet header. */
#define ADDR_OFFSET_HEADER	0x09

/* Size of a data trace address offset packet in bytes. */
#define ADDR_OFFSET_SIZE	3

/* Bitmask for the data trace data value packet header. */
#define DATA_VALUE_HEADER_MASK	0x18

/* Data trace data value packet header. */
#define DATA_VALUE_HEADER	0x10

static bool decode_evtcnt_packet(struct libswo_context *ctx,
		const struct libswo_packet_hw *hw)
{
	if (hw->size != EVTCNT_SIZE) {
		log_warn(ctx, "Event counter packet with invalid size of %zu "
			"bytes.", hw->size);
		return false;
	}

	ctx->packet.hw = *hw;
	ctx->packet.evtcnt.type = LIBSWO_PACKET_TYPE_DWT_EVTCNT;

	ctx->packet.evtcnt.cpi = hw->payload[0] & EVTCNT_CPI_MASK;
	ctx->packet.evtcnt.exc = hw->payload[0] & EVTCNT_EXC_MASK;
	ctx->packet.evtcnt.sleep = hw->payload[0] & EVTCNT_SLEEP_MASK;
	ctx->packet.evtcnt.lsu = hw->payload[0] & EVTCNT_LSU_MASK;
	ctx->packet.evtcnt.fold = hw->payload[0] & EVTCNT_FOLD_MASK;
	ctx->packet.evtcnt.cyc = hw->payload[0] & EVTCNT_CYC_MASK;

	log_dbg(ctx, "Event counter packet decoded.");

	return true;
}

static bool decode_exctrace_packet(struct libswo_context *ctx,
		const struct libswo_packet_hw *hw)
{
	uint16_t exception;
	uint8_t tmp;

	if (hw->size != EXCTRACE_SIZE) {
		log_warn(ctx, "Exception trace packet with invalid size of "
			"%zu bytes.", hw->size);
		return false;
	}

	ctx->packet.hw = *hw;
	ctx->packet.type = LIBSWO_PACKET_TYPE_DWT_EXCTRACE;

	exception = hw->payload[0];
	exception |= (hw->payload[1] & EXCTRACE_EX_MASK) << EXCTRACE_EX_OFFSET;
	tmp = (hw->payload[1] & EXCTRACE_FN_MASK) >> EXCTRACE_FN_OFFSET;

	ctx->packet.exctrace.exception = exception;
	ctx->packet.exctrace.function = tmp;

	log_dbg(ctx, "Exception trace packet decoded.");

	return true;
}

static bool decode_pc_sample_packet(struct libswo_context *ctx,
		const struct libswo_packet_hw *hw)
{
	ctx->packet.hw = *hw;
	ctx->packet.type = LIBSWO_PACKET_TYPE_DWT_PC_SAMPLE;

	if (hw->size == PC_SAMPLE_SLEEP_SIZE) {
		if (hw->value > 0) {
			log_warn(ctx, "Periodic PC sleep packet contains "
				"invalid value: %x.", hw->value);
			return false;
		}

		ctx->packet.pc_sample.sleep = true;
		log_dbg(ctx, "Periodic PC sleep packet decoded.");
	} else if (hw->size == PC_SAMPLE_SIZE) {
		ctx->packet.pc_sample.sleep = false;
		log_dbg(ctx, "Periodic PC sample packet decoded.");
	} else {
		log_warn(ctx, "Periodic PC sample packet with invalid size of "
			"%zu bytes.", hw->size);
		return false;
	}

	ctx->packet.pc_sample.pc = hw->value;

	return true;
}

static bool decode_pc_value_packet(struct libswo_context *ctx,
		const struct libswo_packet_hw *hw)
{
	if (hw->size != PC_VALUE_SIZE) {
		log_warn(ctx, "Data trace PC value packet with invalid size "
			"of %zu bytes.", hw->size);
		return false;
	}

	ctx->packet.hw = *hw;
	ctx->packet.type = LIBSWO_PACKET_TYPE_DWT_PC_VALUE;

	ctx->packet.pc_value.cmpn = (hw->address & CMPN_MASK) >> CMPN_OFFSET;
	ctx->packet.pc_value.pc = hw->value;

	log_dbg(ctx, "Data trace PC value packet decoded.");

	return true;
}

static bool decode_address_offset_packet(struct libswo_context *ctx,
		const struct libswo_packet_hw *hw)
{
	if (hw->size != ADDR_OFFSET_SIZE) {
		log_warn(ctx, "Data trace address offset packet with invalid "
			"size of %zu bytes.", hw->size);
		return false;
	}

	ctx->packet.hw = *hw;
	ctx->packet.type = LIBSWO_PACKET_TYPE_DWT_ADDR_OFFSET;

	ctx->packet.addr_offset.cmpn = (hw->address & CMPN_MASK) >> \
		CMPN_OFFSET;
	ctx->packet.addr_offset.offset = hw->value;

	log_dbg(ctx, "Data trace address offset packet decoded.");

	return true;
}

static bool decode_data_value_packet(struct libswo_context *ctx,
		const struct libswo_packet_hw *hw)
{
	ctx->packet.hw = *hw;
	ctx->packet.type = LIBSWO_PACKET_TYPE_DWT_DATA_VALUE;

	ctx->packet.data_value.wnr = hw->address & WNR_MASK;
	ctx->packet.data_value.cmpn = (hw->address & CMPN_MASK) >> CMPN_OFFSET;
	ctx->packet.data_value.data_value = hw->value;

	log_dbg(ctx, "Data trace data value packet decoded.");

	return true;
}

/**
 * Decode hardware packet as Data Watchpoint and Trace (DWT) packet.
 *
 * @param[in,out] ctx libswo context.
 * @param[in] hw Hardware packet.
 *
 * @retval true Packet decoded successfully.
 * @retval false Packet could not be decoded.
 */
LIBSWO_PRIV bool dwt_decode_packet(struct libswo_context *ctx,
		const struct libswo_packet_hw *hw)
{
	if (hw->address == EVTCNT_ID)
		return decode_evtcnt_packet(ctx, hw);
	else if (hw->address == EXCTRACE_ID)
		return decode_exctrace_packet(ctx, hw);
	else if (hw->address == PC_SAMPLE_ID)
		return decode_pc_sample_packet(ctx, hw);
	else if ((hw->address & PC_VALUE_HEADER_MASK) == PC_VALUE_HEADER)
		return decode_pc_value_packet(ctx, hw);
	else if ((hw->address & ADDR_OFFSET_HEADER_MASK) == ADDR_OFFSET_HEADER)
		return decode_address_offset_packet(ctx, hw);
	else if ((hw->address & DATA_VALUE_HEADER_MASK) == DATA_VALUE_HEADER)
		return decode_data_value_packet(ctx, hw);

	return false;
}
