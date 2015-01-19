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

#include <stdlib.h>
#include <string.h>

#include "libswo.h"
#include "libswo-internal.h"

/**
 * @file
 *
 * Core library functions.
 */

/**
 * Initialize libswo.
 *
 * @param[out] ctx Newly allocated libswo context on success, and undefined on
 * 		   failure.
 * @param[in] buffer Buffer to be used for buffering trace data, or NULL to
 * 		     allocate a new buffer with the given size.
 * @param[in] buffer_size Size of the buffer in bytes.
 *
 * @retval LIBSWO_OK Success.
 * @retval LIBSWO_ERR_ARG Invalid arguments.
 * @retval LIBSWO_ERR_MALLOC Memory allocation error.
 */
LIBSWO_API int libswo_init(struct libswo_context **ctx, uint8_t *buffer,
		size_t buffer_size)
{
	struct libswo_context *context;

	if (!ctx || !buffer_size)
		return LIBSWO_ERR_ARG;

	context = malloc(sizeof(struct libswo_context));

	if (!context)
		return LIBSWO_ERR_MALLOC;

	if (buffer) {
		context->free_buffer = 0;
	} else {
		context->free_buffer = 1;
		buffer = malloc(buffer_size);
	}

	if (!buffer) {
		free(context);
		return LIBSWO_ERR_MALLOC;
	}

	/* Show error and warning messages by default. */
	context->log_level = LIBSWO_LOG_LEVEL_WARNING;

	context->callback = NULL;
	context->cb_user_data = NULL;

	memset(&context->packet, 0, sizeof(union libswo_packet));

	context->buffer = buffer;
	context->size = buffer_size;
	context->read_pos = 0;
	context->write_pos = 0;
	context->bytes_available = 0;

	*ctx = context;

	return LIBSWO_OK;
}

/**
 * Shutdown libswo.
 *
 * @param[in,out] ctx libswo context.
 */
LIBSWO_API void libswo_exit(struct libswo_context *ctx)
{
	if (!ctx)
		return;

	if (ctx->free_buffer)
		free(ctx->buffer);

	free(ctx);
}
