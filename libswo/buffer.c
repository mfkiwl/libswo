/*
 * This file is part of the swodec project.
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

#include <string.h>

#include "libswo-internal.h"

/**
 * @file
 *
 * Internal buffer functions.
 */

/**
 * Write data into the buffer.
 *
 * @param[in,out] ctx libswo context.
 * @param[in] buffer Buffer to write data from.
 * @param[in] length Number of bytes to write.
 *
 * @return 1 on success, or 0 if there is not enough space left to write the
 * 	   requested number of bytes.
 */
int buffer_write(struct libswo_context *ctx, const uint8_t *buffer,
		size_t length)
{
	size_t tmp;

	if (ctx->bytes_available + length > ctx->size)
		return 0;

	if (ctx->write_pos + length > ctx->size) {
		tmp = ctx->size - ctx->write_pos;
		memcpy(ctx->buffer + ctx->write_pos, buffer, tmp);
		memcpy(ctx->buffer, buffer + tmp, length - tmp);
		ctx->write_pos = ctx->write_pos + length - ctx->size;
	} else {
		memcpy(ctx->buffer + ctx->write_pos, buffer, length);
		ctx->write_pos += length;
	}

	ctx->bytes_available += length;

	return 1;
}

/**
 * Peek data from the buffer.
 *
 * @param[in] ctx libswo context.
 * @param[out] buffer Buffer to store peeked data into. Its content is
 * 		      undefined on failure.
 * @param[in] length Number of bytes to peek.
 * @param[in] offset Offset in bytes to start peeking at.
 *
 * @return 1 on success, or 0 if there are not enough data to peek the
 * 	   requested number of bytes.
 */
int buffer_peek(const struct libswo_context *ctx, uint8_t *buffer,
		size_t length, size_t offset)
{
	size_t tmp;

	if (length + offset > ctx->bytes_available)
		return 0;

	if (ctx->read_pos + length + offset > ctx->size) {
		tmp = ctx->size - ctx->read_pos - offset;
		memcpy(buffer, ctx->buffer + ctx->read_pos + offset, tmp);
		memcpy(buffer + tmp, ctx->buffer, length - tmp);
	} else {
		memcpy(buffer, ctx->buffer + ctx->read_pos + offset, length);
	}

	return 1;
}

/**
 * Remove data from the buffer.
 *
 * @param[in,out] ctx libswo context.
 * @param[in] length Number of bytes to remove.
 *
 * @return 1 on success, or 0 if there are not enough bytes in the buffer to be
 * 	   removed.
 */
int buffer_remove(struct libswo_context *ctx, size_t length)
{
	if (length > ctx->bytes_available)
		return 0;

	ctx->bytes_available -= length;
	ctx->read_pos = (ctx->read_pos + length) % ctx->size;

	return 1;
}

/**
 * Flush the buffer.
 *
 * @param[in,out] ctx libswo context.
 */
void buffer_flush(struct libswo_context *ctx)
{
	ctx->bytes_available = 0;
	ctx->read_pos = 0;
	ctx->write_pos = 0;
}
