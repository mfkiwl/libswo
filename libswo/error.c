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

#include "libswo.h"

/**
 * @file
 *
 * Error handling.
 */

/**
 * Return a human-readable description of a libswo error code.
 *
 * @param error_code A libswo error code. See #libswo_error for valid values.
 *
 * @return A string which contains a description of the given error code, or
 * 	   the string <i>unknown error</i> if the error code is not known. The
 * 	   string is null-terminated and must not be free'd by the caller.
 */
LIBSWO_API const char *libswo_strerror(int error_code)
{
	switch (error_code) {
	case LIBSWO_OK:
		return "no error";
	case LIBSWO_ERR:
		return "unspecified error";
	case LIBSWO_ERR_MALLOC:
		return "memory allocation error";
	case LIBSWO_ERR_ARG:
		return "invalid argument";
	default:
		return "unknown error";
	}
}

/**
 * Return the name of a libswo error code.
 *
 * @param error_code A libswo error code. See #libswo_error for valid values.
 *
 * @return A string which contains the name of the given error code, or the
 *         string <i>unknown error code</i> if the error code is not known. The
 *         string is null-terminated and must not be free'd by the caller.
 */
LIBSWO_API const char *libswo_strerror_name(int error_code)
{
	switch (error_code) {
	case LIBSWO_OK:
		return "LIBSWO_OK";
	case LIBSWO_ERR:
		return "LIBSWO_ERR";
	case LIBSWO_ERR_MALLOC:
		return "LIBSWO_ERR_MALLOC";
	case LIBSWO_ERR_ARG:
		return "LIBSWO_ERR_ARG";
	default:
		return "unknown error code";
	}
}
