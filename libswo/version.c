/*
 * This file is part of the libswo project.
 *
 * Copyright (C) 2015 Marc Schink <swo-dev@marcschink.de>
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
 * Package and library version functions.
 */


/**
 * Get the major version number of the libswo package.
 *
 * @return The major version number of the libswo package.
 */
LIBSWO_API int libswo_version_package_get_major(void)
{
	return LIBSWO_VERSION_PACKAGE_MAJOR;
}

/**
 * Get the minor version number of the libswo package.
 *
 * @return The minor version number of the libswo package.
 */
LIBSWO_API int libswo_version_package_get_minor(void)
{
	return LIBSWO_VERSION_PACKAGE_MINOR;
}

/**
 * Get the micro version number of the libswo package.
 *
 * @return The micro version number of the libswo package.
 */
LIBSWO_API int libswo_version_package_get_micro(void)
{
	return LIBSWO_VERSION_PACKAGE_MICRO;
}

/**
 * Get the version number string of the libswo package.
 *
 * @return A string which contains the version number of the libswo package.
 *         The string is null-terminated and must not be free'd by the caller.
 */
LIBSWO_API const char *libswo_version_package_get_string(void)
{
	return LIBSWO_VERSION_PACKAGE_STRING;
}

/**
 * Get the <i>current</i> version number of the libswo libtool interface.
 *
 * @return The <i>current</i> version number of the libswo libtool interface.
 */
LIBSWO_API int libswo_version_library_get_current(void)
{
	return LIBSWO_VERSION_LIBRARY_CURRENT;
}

/**
 * Get the <i>revision</i> version number of the libswo libtool interface.
 *
 * @return The <i>revision</i> version number of the libswo libtool interface.
 */
LIBSWO_API int libswo_version_library_get_revision(void)
{
	return LIBSWO_VERSION_LIBRARY_REVISION;
}

/**
 * Get the <i>age</i> version number of the libswo libtool interface.
 *
 * @return The <i>age</i> version number of the libswo libtool interface.
 */
LIBSWO_API int libswo_version_library_get_age(void)
{
	return LIBSWO_VERSION_LIBRARY_AGE;
}

/**
 * Get the version number string of the libswo libtool interface.
 *
 * @return A string which contains the version number of the libswo libtool
 *         interface. The string is null-terminated and must not be free'd by
 *         the caller.
 */
LIBSWO_API const char *libswo_version_library_get_string(void)
{
	return LIBSWO_VERSION_LIBRARY_STRING;
}
