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

#include <sstream>

#include "libswocxx.h"

namespace libswo
{

Overflow::Overflow(const struct libswo_packet_of *packet)
{
	_packet = *((const union libswo_packet *)packet);
}

Overflow::Overflow(const union libswo_packet *packet)
{
	_packet = *packet;
}

const std::string Overflow::to_string(void) const
{
	std::stringstream ss;

	ss << "Overflow (size = " << get_size() << " bytes)";

	return ss.str();
}

}
