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

/** @private */
static void log_vprintf(struct libswo_context *ctx, int level,
		const char *format, va_list args)
{
	/*
	 * Filter out messages with higher verbosity than the verbosity of the
	 * current log level.
	 */
	if (level > ctx->log_level)
		return;

	fprintf(stderr, "libswo: ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
}

/** @private */
LIBSWO_PRIV void log_err(struct libswo_context *ctx, const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	log_vprintf(ctx, LIBSWO_LOG_LEVEL_ERROR, format, args);
	va_end(args);
}

/** @private */
LIBSWO_PRIV void log_warn(struct libswo_context *ctx, const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	log_vprintf(ctx, LIBSWO_LOG_LEVEL_WARNING, format, args);
	va_end(args);
}

/** @private */
LIBSWO_PRIV void log_info(struct libswo_context *ctx, const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	log_vprintf(ctx, LIBSWO_LOG_LEVEL_INFO, format, args);
	va_end(args);
}

/** @private */
LIBSWO_PRIV void log_dbg(struct libswo_context *ctx, const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	log_vprintf(ctx, LIBSWO_LOG_LEVEL_DEBUG, format, args);
	va_end(args);
}
