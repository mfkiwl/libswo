/*
 * This file is part of the libswo project.
 *
 * Copyright (C) 2014 Marc Schink <swo-dev@marcschink.de>
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

#ifndef LIBSWO_LIBSWO_INTERNAL_H
#define LIBSWO_LIBSWO_INTERNAL_H

#include <stdint.h>

#include "libswo.h"

/**
 * @file
 *
 * Internal libswo header file.
 */

/** Calculate the minimum of two numeric values. */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct libswo_context {
	/** Current log level. */
	int log_level;

	/** Decoder callback function. */
	libswo_decoder_callback callback;

	/** User data to be passed to the decoder callback function. */
	void *cb_user_data;

	/** Last decoded packet. */
	union libswo_packet packet;

	/** Buffer. */
	uint8_t *buffer;

	/** Buffer size. */
	size_t size;

	/**
	 * Indicates whether the buffer was allocated during initialization and
	 * must be free'ed on shutdown.
	 */
	int free_buffer;

	/** Current read position of the buffer. */
	size_t read_pos;

	/** Current write position of the buffer. */
	size_t write_pos;

	/** Number of bytes in the buffer. */
	size_t bytes_available;
};

int buffer_write(struct libswo_context *ctx, const uint8_t *buffer,
		size_t length);
int buffer_peek(const struct libswo_context *ctx, uint8_t *buffer,
		size_t length, size_t offset);
int buffer_remove(struct libswo_context *ctx, size_t length);
void buffer_flush(struct libswo_context *ctx);

void log_err(struct libswo_context *ctx, const char *format, ...);
void log_warn(struct libswo_context *ctx, const char *format, ...);
void log_info(struct libswo_context *ctx, const char *format, ...);
void log_dbg(struct libswo_context *ctx, const char *format, ...);

#endif /* LIBSWO_LIBSWO_INTERNAL_H */
