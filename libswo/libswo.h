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

#ifndef LIBSWO_LIBSWO_H
#define LIBSWO_LIBSWO_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file
 *
 * Public libswo header file to be used by applications.
 */

/** Error codes returned by libswo functions. */
enum libswo_error {
	/** No error. */
	LIBSWO_OK = 0,
	/** Unspecific error. */
	LIBSWO_ERR = -1,
	/** Memory allocation error. */
	LIBSWO_ERR_MALLOC = -2,
	/** Invalid argument. */
	LIBSWO_ERR_ARG = -3
};

/** libswo log levels. */
enum libswo_log_level {
	/** Output no messages. */
	LIBSWO_LOG_LEVEL_NONE = 0,
	/** Output error messages. */
	LIBSWO_LOG_LEVEL_ERROR = 1,
	/** Output warnings. */
	LIBSWO_LOG_LEVEL_WARNING = 2,
	/** Output informational messages. */
	LIBSWO_LOG_LEVEL_INFO = 3,
	/** Output debug messages. */
	LIBSWO_LOG_LEVEL_DEBUG = 4
};


/** Default libswo log domain. */
#define LIBSWO_LOG_DOMAIN_DEFAULT	"libswo: "

/** Maximum length of a libswo log domain in bytes. */
#define LIBSWO_LOG_DOMAIN_MAX_LENGTH	32

/** Packet types. */
enum libswo_packet_type {
	/** Synchronization packet. */
	LIBSWO_PACKET_TYPE_SYNC = 0,
	/** Overflow packet. */
	LIBSWO_PACKET_TYPE_OVERFLOW = 1,
	/** Local timestamp packet. */
	LIBSWO_PACKET_TYPE_LTS = 2,
	/** Global timestamp (GTS1) packet. */
	LIBSWO_PACKET_TYPE_GTS1 = 3,
	/** Global timestamp (GTS2) packet. */
	LIBSWO_PACKET_TYPE_GTS2 = 4,
	/** Extension packet. */
	LIBSWO_PACKET_TYPE_EXT = 5,
	/** Instrumentation packet. */
	LIBSWO_PACKET_TYPE_INST = 6,
	/** Hardware source packet. */
	LIBSWO_PACKET_TYPE_HW = 7,
	/** Unknown data packet. */
	LIBSWO_PACKET_TYPE_UNKNOWN = 8
};

/** Local timestamp relation information. */
enum libswo_lts_relation {
	/** Source and timestamp packet are synchronous. */
	LIBSWO_LTS_REL_SYNC = 0,
	/** Timestamp packet is delayed relative to the source packet. */
	LIBSWO_LTS_REL_TS = 1,
	/** Source packet is delayed relative to the local timestamp packet. */
	LIBSWO_LTS_REL_SRC = 2,
	/** Source and timestamp packet are both delayed. */
	LIBSWO_LTS_REL_BOTH = 3
};

/** Extension sources. */
enum libswo_ext_source {
	/** Instrumentation Trace Macrocell (ITM) extension packet. */
	LIBSWO_EXT_SRC_ITM = 0,
	/** Hardware source extension packet. */
	LIBSWO_EXT_SRC_HW = 1
};

/** Decoder flags. */
enum libswo_decoder_flags {
	/**
	 * Indicates the end of the data stream to the decoder.
	 *
	 * If this flag is set, the decoder treats all remaining and incomplete
	 * packets as unknown data.
	 */
	LIBSWO_DF_EOS = (1 << 0)
};

/** Maximum payload size of a packet in bytes. */
#define LIBSWO_MAX_PAYLOAD_SIZE	4

/**
 * Common fields packet.
 *
 * Helper which contains the fields that are common to all packets.
 */
struct libswo_packet_any {
	/** Packet type. */
	enum libswo_packet_type type;
	/**
	 * Packet size information.
	 *
	 * Interpretation of this value depends on the specific packet type.
	 */
	size_t size;
};

/** Synchronization packet. */
struct libswo_packet_sync {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size in bits. */
	size_t size;
};

/** Overflow packet. */
struct libswo_packet_of {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
};

/** Local timestamp packet. */
struct libswo_packet_lts {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/**
	 * Indicates the relationship between the generation of local timestamp
	 * packet and the corresponding source packet.
	 */
	enum libswo_lts_relation relation;
	/** Local timestamp value. */
	uint32_t value;
};

/** Global timestamp (GTS1) packet. */
struct libswo_packet_gts1 {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Low-order bits [25:0] of the global timestamp value. */
	uint32_t value;
	/**
	 * Indicates if the system has asserted the clock change input to the
	 * processor since the last emitted global timestamp packet.
	 */
	bool clkch;
	/**
	 * Indicates if the high-order global timestamp bits [47:26] changed
	 * since the last emitted global timestamp (GTS2) packet.
	 */
	bool wrap;
};

/** Global timestamp (GTS2) packet. */
struct libswo_packet_gts2 {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** High-order bits [47:26] of the global timestamp value. */
	uint32_t value;
};

/** Extension packet. */
struct libswo_packet_ext {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Source of the extension packet. */
	enum libswo_ext_source source;
	/** Extension information. */
	uint32_t value;
};

/** Instrumentation packet. */
struct libswo_packet_inst {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Port number. */
	uint8_t address;
	/** Instrumentation data. */
	uint8_t payload[LIBSWO_MAX_PAYLOAD_SIZE];
	/** Integer representation of the payload. */
	uint32_t value;
};

/** Hardware source packet. */
struct libswo_packet_hw {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Address. */
	uint8_t address;
	/** Hardware source data. */
	uint8_t payload[LIBSWO_MAX_PAYLOAD_SIZE];
	/** Integer representation of the payload. */
	uint32_t value;
};

/**
 * Unknown data packet.
 *
 * This packet represents data which could not be decoded.
 */
struct libswo_packet_unknown {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size in bytes. */
	size_t size;
};

/** Union of all packet types. */
union libswo_packet {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Common fields packet. */
	struct libswo_packet_any any;
	/** Synchronization packet. */
	struct libswo_packet_sync sync;
	/** Overflow packet. */
	struct libswo_packet_of of;
	/** Local timestamp packet. */
	struct libswo_packet_lts lts;
	/** Global timestamp (GTS1) packet. */
	struct libswo_packet_gts1 gts1;
	/** Global timestamp (GTS2) packet. */
	struct libswo_packet_gts2 gts2;
	/** Extension packet. */
	struct libswo_packet_ext ext;
	/** Instrumentation packet. */
	struct libswo_packet_inst inst;
	/** Hardware source packet. */
	struct libswo_packet_hw hw;
	/** Unknown data packet. */
	struct libswo_packet_unknown unknown;
};

/**
 * @struct libswo_context
 *
 * Opaque structure representing a libswo context.
 */
struct libswo_context;

/**
 * Decoder callback function type.
 *
 * @param[in,out] ctx libswo context.
 * @param[out] packet Decoded packet.
 * @param[in,out] user_data User data passed to the callback function.
 *
 * @retval true Continue decoding.
 * @retval false Stop decoding.
 */
typedef int (*libswo_decoder_callback)(struct libswo_context *ctx,
		const union libswo_packet *packet, void *user_data);

/** Log callback function type. */
typedef int (*libswo_log_callback)(struct libswo_context *ctx, int level,
                const char *format, va_list args, void *user_data);

/** Macro to mark private libswo symbol. */
#if defined(_WIN32) || defined(__MSYS__)
#define LIBSWO_PRIV
#else
#define LIBSWO_PRIV __attribute__ ((visibility ("hidden")))
#endif

/** Macro to mark public libswo API symbol. */
#ifdef _WIN32
#define LIBSWO_API
#else
#define LIBSWO_API __attribute__ ((visibility ("default")))
#endif

/*--- core.c ----------------------------------------------------------------*/

LIBSWO_API int libswo_init(struct libswo_context **ctx, uint8_t *buffer,
		size_t buffer_size);
LIBSWO_API void libswo_exit(struct libswo_context *ctx);

/*--- decoder.c -------------------------------------------------------------*/

LIBSWO_API int libswo_feed(struct libswo_context *ctx, const uint8_t *buffer,
		size_t length);
LIBSWO_API int libswo_decode(struct libswo_context *ctx, uint32_t flags);
LIBSWO_API int libswo_set_callback(struct libswo_context *ctx,
		libswo_decoder_callback callback, void *user_data);

/*--- error.c ---------------------------------------------------------------*/

LIBSWO_API const char *libswo_strerror(int error_code);
LIBSWO_API const char *libswo_strerror_name(int error_code);

/*--- log.c -----------------------------------------------------------------*/

LIBSWO_API int libswo_log_set_level(struct libswo_context *ctx, int level);
LIBSWO_API int libswo_log_get_level(const struct libswo_context *ctx);
LIBSWO_API int libswo_log_set_callback(struct libswo_context *ctx,
		libswo_log_callback callback, void *user_data);
LIBSWO_API int libswo_log_set_domain(struct libswo_context *ctx,
		const char *domain);
LIBSWO_API const char *libswo_log_get_domain(const struct libswo_context *ctx);

/*--- version.c -------------------------------------------------------------*/

LIBSWO_API int libswo_version_package_get_major(void);
LIBSWO_API int libswo_version_package_get_minor(void);
LIBSWO_API int libswo_version_package_get_micro(void);
LIBSWO_API const char *libswo_version_package_get_string(void);
LIBSWO_API int libswo_version_lib_get_current(void);
LIBSWO_API int libswo_version_lib_get_revision(void);
LIBSWO_API int libswo_version_lib_get_age(void);
LIBSWO_API const char *libswo_version_lib_get_string(void);

#include "version.h"

#ifdef __cplusplus
}
#endif

#endif /* LIBSWO_LIBSWO_H */
