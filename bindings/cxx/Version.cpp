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

#include "libswocxx.h"

namespace libswo
{

int Version::get_package_major(void)
{
	return libswo_version_package_get_major();
}

int Version::get_package_minor(void)
{
	return libswo_version_package_get_minor();
}

int Version::get_package_micro(void)
{
	return libswo_version_package_get_micro();
}

string Version::get_package_string(void)
{
	return libswo_version_package_get_string();
}

int Version::get_lib_current(void)
{
	return libswo_version_lib_get_current();
}

int Version::get_lib_revision(void)
{
	return libswo_version_lib_get_revision();
}

int Version::get_lib_age(void)
{
	return libswo_version_lib_get_age();
}

string Version::get_lib_string(void)
{
	return libswo_version_lib_get_string();
}

}
