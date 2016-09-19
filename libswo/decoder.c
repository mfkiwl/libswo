/*
 * This file is part of the libswo project.
 *
 * Copyright (C) 2014-2015 Marc Schink <swo-dev@marcschink.de>
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

#include "libswo.h"
#include "libswo-internal.h"

/**
 * @file
 *
 * Decoder functions.
 */

/** @cond PRIVATE */
/** Synchronization packet header. */
#define SYNC_HEADER		0x00

/** Minimal number of 0 bits required for a synchronization packet. */
#define SYNC_MIN_BITS		47

/** Overflow packet header. */
#define OVERFLOW_HEADER		0x70

/** Local timestamp packet header. */
#define LTS_HEADER		0xc0

/** Bitmask for the timestamp of a local timestamp (LTS1) packet. */
#define LTS1_TS_MASK		0xfffffff

/** Bitmask for the relation information of a local timestamp (LTS1) packet. */
#define LTS1_TC_MASK		0x30

/** Offset of the relation information of a local timestamp (LTS1) packet. */
#define LTS1_TC_OFFSET		4

/** Bitmask for the timestamp of a local timestamp (LTS2) packet. */
#define LTS2_TS_MASK		0x70

/** Offset of the timestamp of local timestamp (LTS2) packet. */
#define LTS2_TS_OFFSET		4

/** Global timestamp packet header. */
#define GTS_HEADER		0x94

/** Bitmask for the global timestamp packet header. */
#define GTS_HEADER_MASK		0xdf

/** Bitmask for the type of a global timestamp packet. */
#define GTS_TYPE_MASK		0x20

/** Bitmask for the timestamp of a global timestamp (GTS1) packet. */
#define GTS1_TS_MASK		0x03ffffff

/** Bitmask for the clkch bit of a global timestamp (GTS1) packet. */
#define GTS1_CLKCH_MASK		0x4000000

/** Bitmask for the wrap bit of a global timestamp (GTS1) packet. */
#define GTS1_WRAP_MASK		0x8000000

/** Payload size of a global timestamp (GTS2) packet in bytes. */
#define GTS2_PAYLOAD_SIZE	4

/** Bitmask for the timestamp of a global timestamp (GTS2) packet. */
#define GTS2_TS_MASK		0x3fffff

/** Extension packet header. */
#define EXT_HEADER		0x08

/** Bitmask for the extension packet header. */
#define EXT_HEADER_MASK		0x0b

/** Bitmask for the source bit of an extension packet. */
#define EXT_SRC_MASK		0x04

/** Bitmask for the extension information of an extension packet header. */
#define EXT_TS_MASK		0x70

/** Offset of the extension information of an extension packet header. */
#define EXT_TS_OFFSET		4

/** Bitmask for the payload size of a source packet. */
#define SRC_SIZE_MASK		0x03

/** Bitmask for the type of a source packet. */
#define SRC_TYPE_MASK		0x04

/** Bitmask for the address of a source packet. */
#define SRC_ADDR_MASK		0xf8

/** Offset of the address of a source packet. */
#define SRC_ADDR_OFFSET		3

/** Bitmask for the continuation bit. */
#define C_MASK			0x80
/** @endcond */

static int decode_packet_type(uint8_t header)
{
	if (header == SYNC_HEADER)
		return LIBSWO_PACKET_TYPE_SYNC;

	if (header == OVERFLOW_HEADER)
		return LIBSWO_PACKET_TYPE_OVERFLOW;

	if (!(header & ~LTS2_TS_MASK))
		return LIBSWO_PACKET_TYPE_LTS;

	if ((header & ~LTS1_TC_MASK) == LTS_HEADER)
		return LIBSWO_PACKET_TYPE_LTS;

	if ((header & EXT_HEADER_MASK) == EXT_HEADER)
		return LIBSWO_PACKET_TYPE_EXT;

	if ((header & GTS_HEADER_MASK) == GTS_HEADER) {
		if (header & GTS_TYPE_MASK)
			return LIBSWO_PACKET_TYPE_GTS2;
		else
			return LIBSWO_PACKET_TYPE_GTS1;
	}

	if (header & SRC_SIZE_MASK) {
		if (header & SRC_TYPE_MASK)
			return LIBSWO_PACKET_TYPE_HW;
		else
			return LIBSWO_PACKET_TYPE_INST;
	}

	return LIBSWO_PACKET_TYPE_UNKNOWN;
}

static int decode_cond_payload(struct libswo_context *ctx, uint32_t *value)
{
	unsigned int i;
	unsigned int n;
	uint8_t dummy;
	uint32_t tmp;

	n = 0;
	tmp = 0;

	for (i = 0; i < LIBSWO_MAX_PAYLOAD_SIZE - 1; i++) {
		if (!buffer_peek(ctx, &dummy, 1, 1 + i))
			return 0;

		tmp |= ((dummy & ~C_MASK) << (i * 7));
		n++;

		if (!(dummy & C_MASK))
			break;
	}

	if (dummy & C_MASK) {
		if (!buffer_peek(ctx, &dummy, 1, 1 + i))
			return 0;

		tmp |= (dummy << (i * 7));
		n++;
	}

	*value = tmp;

	return n;
}

static uint32_t decode_payload(const uint8_t *buffer, uint8_t size)
{
	uint32_t tmp;
	unsigned int i;

	tmp = 0;

	for (i = 0; i < size; i++)
		tmp |= (buffer[i] << (i * 8));

	return tmp;
}

static bool decode_sync_packet(struct libswo_context *ctx)
{
	size_t i;
	size_t j;
	size_t num_bits;
	uint8_t tmp;

	num_bits = 8;

	for (i = 0; ; i++) {
		if (!buffer_peek(ctx, &tmp, 1, 1 + i)) {
			log_dbg(ctx, "Not enough bytes available to decode "
				"synchronization packet.");
			return false;
		}

		if (tmp)
			break;

		num_bits += 8;
	}

	for (j = 0; j < 8; j++, num_bits++)
		if (tmp & (1 << j))
			break;

	if (num_bits < SYNC_MIN_BITS) {
		log_dbg(ctx, "Not enough bits for synchronization packet. "
			"Treating all zero bytes as unknown data.");

		ctx->packet.type = LIBSWO_PACKET_TYPE_UNKNOWN;
		ctx->packet.unknown.size = i + 1;
		return true;
	}

	ctx->packet.type = LIBSWO_PACKET_TYPE_SYNC;
	ctx->packet.sync.size = num_bits + 1;

	log_dbg(ctx, "Synchronization packet decoded.");

	return true;
}

static bool decode_overflow_packet(struct libswo_context *ctx)
{
	ctx->packet.type = LIBSWO_PACKET_TYPE_OVERFLOW;
	ctx->packet.of.size = 1;

	log_dbg(ctx, "Overflow packet decoded.");

	return true;
}

static bool decode_lts_packet(struct libswo_context *ctx, uint8_t header)
{
	int ret;
	uint32_t value;

	ctx->packet.type = LIBSWO_PACKET_TYPE_LTS;

	if (!(header & C_MASK)) {
		ctx->packet.lts.size = 1;
		ctx->packet.lts.value = (header & LTS2_TS_MASK) >> \
			LTS2_TS_OFFSET;
		ctx->packet.lts.relation = LIBSWO_LTS_REL_SYNC;

		log_dbg(ctx, "Local timestamp (LTS2) packet decoded.");
		return true;
	}

	ret = decode_cond_payload(ctx, &value);

	if (ret > 0) {
		ctx->packet.lts.size = ret + 1;
		ctx->packet.lts.value = value & LTS1_TS_MASK;
		ctx->packet.lts.relation = (header & LTS1_TC_MASK) >> \
			LTS1_TC_OFFSET;

		if (value & ~LTS1_TS_MASK)
			log_warn(ctx, "Local timestamp (LTS1) packet contains "
				"invalid trailing bit.");

		log_dbg(ctx, "Local timestamp (LTS1) packet decoded.");
		return true;
	}

	log_dbg(ctx, "Not enough bytes available to decode local timestamp "
		"packet.");

	return false;
}

static bool decode_gts1_packet(struct libswo_context *ctx)
{
	int ret;
	uint32_t value;

	ret = decode_cond_payload(ctx, &value);

	if (ret > 0) {
		ctx->packet.type = LIBSWO_PACKET_TYPE_GTS1;
		ctx->packet.gts1.size = ret + 1;
		ctx->packet.gts1.value = value & GTS1_TS_MASK;

		if (value & GTS1_CLKCH_MASK)
			ctx->packet.gts1.clkch = true;
		else
			ctx->packet.gts1.clkch = false;

		if (value & GTS1_WRAP_MASK)
			ctx->packet.gts1.wrap = true;
		else
			ctx->packet.gts1.wrap = false;

		if (value & ~(GTS1_WRAP_MASK | GTS1_CLKCH_MASK | GTS1_TS_MASK))
			log_warn(ctx, "Global timestamp (GTS1) packet "
				"contains invalid trailing bit.");

		log_dbg(ctx, "Global timestamp (GTS1) packet decoded.");
		return true;
	}

	log_dbg(ctx, "Not enough bytes available to decode global timestamp "
		"(GTS1) packet.");

	return false;
}

static bool decode_gts2_packet(struct libswo_context *ctx)
{
	int ret;
	uint32_t value;

	ret = decode_cond_payload(ctx, &value);

	if (ret > 0) {
		if (ret != GTS2_PAYLOAD_SIZE) {
			log_warn(ctx, "Global timestamp (GTS2) packet with "
				"invalid payload size of %u bytes.", ret);

			ctx->packet.type = LIBSWO_PACKET_TYPE_UNKNOWN;
			ctx->packet.unknown.size = 1;
			return true;
		}

		ctx->packet.type = LIBSWO_PACKET_TYPE_GTS2;
		ctx->packet.gts2.size = ret + 1;
		ctx->packet.gts2.value = value & GTS2_TS_MASK;

		if (value & ~GTS2_TS_MASK)
			log_warn(ctx, "Global timestamp (GTS2) packet "
				"contains invalid trailing bits.");

		log_dbg(ctx, "Global timestamp (GTS2) packet decoded.");
		return true;
	}

	log_dbg(ctx, "Not enough bytes available to decode global timestamp "
		"(GTS2) packet.");

	return false;
}

static bool decode_ext_packet(struct libswo_context *ctx, uint8_t header)
{
	int ret;
	size_t size;
	uint32_t value;
	uint32_t tmp;

	size = 1;
	value = (header & EXT_TS_MASK) >> EXT_TS_OFFSET;

	if (header & C_MASK) {
		ret = decode_cond_payload(ctx, &tmp);

		if (ret > 0) {
			value |= tmp << 3;
			size += ret;
		} else {
			log_dbg(ctx, "Not enough bytes available to decode "
				"extension packet.");
			return false;
		}
	}

	ctx->packet.type = LIBSWO_PACKET_TYPE_EXT;
	ctx->packet.ext.size = size;
	ctx->packet.ext.value = value;

	if (header & EXT_SRC_MASK)
		ctx->packet.ext.source = LIBSWO_EXT_SRC_HW;
	else
		ctx->packet.ext.source = LIBSWO_EXT_SRC_ITM;

	log_dbg(ctx, "Extension packet decoded.");

	return true;
}

static bool decode_inst_packet(struct libswo_context *ctx, uint8_t header)
{
	uint8_t payload_size;

	payload_size = 1 << ((header & SRC_SIZE_MASK) - 1);

	if (!buffer_peek(ctx, ctx->packet.inst.payload, payload_size, 1)) {
		log_dbg(ctx, "Not enough bytes available to decode "
			"instrumentation packet.");
		return false;
	}

	ctx->packet.type = LIBSWO_PACKET_TYPE_INST;
	ctx->packet.inst.size = payload_size + 1;
	ctx->packet.inst.address = (header & SRC_ADDR_MASK) >> SRC_ADDR_OFFSET;
	ctx->packet.inst.value = decode_payload(ctx->packet.inst.payload,
		payload_size);

	log_dbg(ctx, "Instrumentation packet decoded.");

	return true;
}

static bool decode_hw_packet(struct libswo_context *ctx, uint8_t header)
{
	uint8_t payload_size;
	struct libswo_packet_hw hw;

	payload_size = 1 << ((header & SRC_SIZE_MASK) - 1);

	if (!buffer_peek(ctx, hw.payload, payload_size, 1)) {
		log_dbg(ctx, "Not enough bytes available to decode hardware "
			"source packet.");
		return false;
	}

	hw.type = LIBSWO_PACKET_TYPE_HW;
	hw.size = payload_size + 1;
	hw.address = (header & SRC_ADDR_MASK) >> SRC_ADDR_OFFSET;
	hw.value = decode_payload(hw.payload, payload_size);

	if (!dwt_decode_packet(ctx, &hw)) {
		log_dbg(ctx, "Hardware source packet decoded.");
		ctx->packet.hw = hw;
	}

	return true;
}

static bool handle_unknown_header(struct libswo_context *ctx, uint8_t header)
{
	log_dbg(ctx, "Unknown header: %02x.", header);
	ctx->packet.type = LIBSWO_PACKET_TYPE_UNKNOWN;
	ctx->packet.unknown.size = 1;

	return true;
}

static int handle_eos(struct libswo_context *ctx)
{
	int ret;
	size_t tmp;

	log_dbg(ctx, "Treating %zu remaining bytes as unknown data.",
		ctx->bytes_available);

	ctx->packet.type = LIBSWO_PACKET_TYPE_UNKNOWN;

	while (ctx->bytes_available > 0) {
		tmp = MIN(sizeof(ctx->packet.any.data), ctx->bytes_available);

		ctx->packet.unknown.size = tmp;
		buffer_read(ctx, ctx->packet.unknown.data, tmp, 0);

		if (ctx->callback)
			ret = ctx->callback(ctx, &ctx->packet,
				ctx->cb_user_data);
		else
			ret = true;

		if (ret < 0) {
			return ret;
		} else if (!ret) {
			log_dbg(ctx, "Decoding stopped by callback function.");
			return LIBSWO_OK;
		}
	}

	return LIBSWO_OK;
}

static int handle_packet(struct libswo_context *ctx)
{
	int ret;
	size_t tmp;

	if (ctx->packet.type == LIBSWO_PACKET_TYPE_SYNC) {
		tmp = (ctx->packet.sync.size + 7) / 8;
	} else {
		tmp = ctx->packet.any.size;
		buffer_peek(ctx, ctx->packet.any.data, tmp, 0);
	}

	if (ctx->callback)
		ret = ctx->callback(ctx, &ctx->packet, ctx->cb_user_data);
	else
		ret = true;

	buffer_remove(ctx, tmp);

	return ret;
}

/**
 * Feed the decoder with trace data.
 *
 * @param[in,out] ctx libswo context.
 * @param[in] buffer Buffer with trace data to feed the decoder with.
 * @param[in] length Number of bytes to feed.
 *
 * @retval LIBSWO_OK Success.
 * @retval LIBSWO_ERR Other error conditions.
 * @retval LIBSWO_ERR_ARG Invalid arguments.
 */
LIBSWO_API int libswo_feed(struct libswo_context *ctx, const uint8_t *buffer,
		size_t length)
{
	if (!ctx || !buffer)
		return LIBSWO_ERR_ARG;

	if (buffer_write(ctx, buffer, length))
		return LIBSWO_OK;

	return LIBSWO_ERR;
}

/**
 * Decode the trace data.
 *
 * @param[in,out] ctx libswo context.
 * @param[in] flags Decoder flags, see #libswo_decoder_flags for a description.
 *
 * @retval LIBSWO_OK Success.
 * @retval LIBSWO_ERR Other error conditions.
 * @retval LIBSWO_ERR_ARG Invalid arguments.
 */
LIBSWO_API int libswo_decode(struct libswo_context *ctx, uint32_t flags)
{
	int ret;
	uint8_t header;
	int packet_type;

	if (!ctx)
		return LIBSWO_ERR_ARG;

	while (true) {
		if (!buffer_peek(ctx, &header, 1, 0))
			break;

		packet_type = decode_packet_type(header);

		switch (packet_type) {
		case LIBSWO_PACKET_TYPE_SYNC:
			ret = decode_sync_packet(ctx);
			break;
		case LIBSWO_PACKET_TYPE_OVERFLOW:
			ret = decode_overflow_packet(ctx);
			break;
		case LIBSWO_PACKET_TYPE_LTS:
			ret = decode_lts_packet(ctx, header);
			break;
		case LIBSWO_PACKET_TYPE_EXT:
			ret = decode_ext_packet(ctx, header);
			break;
		case LIBSWO_PACKET_TYPE_GTS1:
			ret = decode_gts1_packet(ctx);
			break;
		case LIBSWO_PACKET_TYPE_GTS2:
			ret = decode_gts2_packet(ctx);
			break;
		case LIBSWO_PACKET_TYPE_INST:
			ret = decode_inst_packet(ctx, header);
			break;
		case LIBSWO_PACKET_TYPE_HW:
			ret = decode_hw_packet(ctx, header);
			break;
		case LIBSWO_PACKET_TYPE_UNKNOWN:
			ret = handle_unknown_header(ctx, header);
			break;
		default:
			log_err(ctx, "Invalid packet type %i for header %02x.",
				packet_type, header);
			return LIBSWO_ERR;
		}

		if (!ret)
			break;

		ret = handle_packet(ctx);

		if (ret < 0) {
			return LIBSWO_ERR;
		} else if (!ret) {
			log_dbg(ctx, "Decoding stopped by callback function.");
			return LIBSWO_OK;
		}
	}

	if (flags & LIBSWO_DF_EOS) {
		log_dbg(ctx, "End of stream reached.");

		if (ctx->bytes_available > 0) {
			if (handle_eos(ctx) < 0)
				return LIBSWO_ERR;
		}
	}

	return LIBSWO_OK;
}

/**
 * Set the decoder callback function.
 *
 * @param[in,out] ctx libswo context.
 * @param[in] callback Callback function to be used, or NULL to disable
 *                     callback invocation.
 * @param[in] user_data User data to be passed to the callback function.
 *
 * @retval LIBSWO_OK Success.
 * @retval LIBSWO_ERR_ARG Invalid argument.
 */
LIBSWO_API int libswo_set_callback(struct libswo_context *ctx,
		libswo_decoder_callback callback, void *user_data)
{
	if (!ctx)
		return LIBSWO_ERR_ARG;

	ctx->callback = callback;
	ctx->cb_user_data = user_data;

	return LIBSWO_OK;
}
