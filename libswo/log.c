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

#include <stdio.h>
#include <stdarg.h>

#include "libswo.h"
#include "libswo-internal.h"

/**
 * @file
 *
 * Logging functions.
 */

/**
 * Set the libswo log level.
 *
 * @param[in,out] ctx libswo context.
 * @param[in] level Log level to set. See #libswo_log_level for valid values.
 *
 * @retval LIBSWO_OK Success.
 * @retval LIBSWO_ERR_ARG Invalid arguments.
 */
LIBSWO_API int libswo_log_set_level(struct libswo_context *ctx, int level)
{
	if (!ctx)
		return LIBSWO_ERR_ARG;

	if (level < LIBSWO_LOG_LEVEL_NONE || level > LIBSWO_LOG_LEVEL_DEBUG)
		return LIBSWO_ERR_ARG;

	ctx->log_level = level;

	return LIBSWO_OK;
}

/**
 * Get the libswo log level.
 *
 * @param[in,out] ctx libswo context.
 *
 * @return The current log level on success, or a negative error code
 * 	   on failure. See #libswo_log_level for a description of each
 * 	   individual log level.
 */
LIBSWO_API int libswo_log_get_level(const struct libswo_context *ctx)
{
	if (!ctx)
		return LIBSWO_ERR_ARG;

	return ctx->log_level;
}

/**
 * Set the libswo log callback function.
 *
 * @param[in,out] ctx libswo context.
 * @param[in] callback Callback function to use, or NULL to use the default log
 * 		       function.
 * @param[in] user_data User data to be passed to the callback function.
 *
 * @retval LIBSWO_OK Success.
 * @retval LIBSWO_ERR_ARG Invalid arguments.
 */
LIBSWO_API int libswo_log_set_callback(struct libswo_context *ctx,
		libswo_log_callback callback, void *user_data)
{
	if (!ctx)
		return LIBSWO_ERR_ARG;

	if (callback) {
		ctx->log_callback = callback;
		ctx->log_cb_user_data = user_data;
	} else {
		ctx->log_callback = &log_vprintf;
		ctx->log_cb_user_data = NULL;
	}

	return LIBSWO_OK;
}

/** @private */
LIBSWO_PRIV int log_vprintf(struct libswo_context *ctx, int level,
		const char *format, va_list args, void *user_data)
{
	(void)user_data;

	/*
	 * Filter out messages with higher verbosity than the verbosity of the
	 * current log level.
	 */
	if (level > ctx->log_level)
		return 0;

	fprintf(stderr, "libswo: ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");

	return 0;
}

/** @private */
LIBSWO_PRIV void log_err(struct libswo_context *ctx, const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	ctx->log_callback(ctx, LIBSWO_LOG_LEVEL_ERROR, format, args,
		ctx->log_cb_user_data);
	va_end(args);
}

/** @private */
LIBSWO_PRIV void log_warn(struct libswo_context *ctx, const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	ctx->log_callback(ctx, LIBSWO_LOG_LEVEL_WARNING, format, args,
		ctx->log_cb_user_data);
	va_end(args);
}

/** @private */
LIBSWO_PRIV void log_info(struct libswo_context *ctx, const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	ctx->log_callback(ctx, LIBSWO_LOG_LEVEL_INFO, format, args,
		ctx->log_cb_user_data);
	va_end(args);
}

/** @private */
LIBSWO_PRIV void log_dbg(struct libswo_context *ctx, const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	ctx->log_callback(ctx, LIBSWO_LOG_LEVEL_DEBUG, format, args,
		ctx->log_cb_user_data);
	va_end(args);
}
