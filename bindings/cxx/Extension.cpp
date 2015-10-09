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

Extension::Extension(const struct libswo_packet_ext *packet)
{
	_packet = *((const union libswo_packet *)packet);
}

Extension::Extension(const union libswo_packet *packet)
{
	_packet = *packet;
}

enum ExtensionSource Extension::get_source(void) const
{
	return static_cast<enum ExtensionSource>(_packet.ext.source);
}

uint32_t Extension::get_value(void) const
{
	return _packet.ext.value;
}

}
