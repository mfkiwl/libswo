/*
 * This file is part of the libswo project.
 *
 * Copyright (C) 2016 Marc Schink <swo-dev@marcschink.de>
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
PCValue::PCValue(const struct libswo_packet_dwt_pc_value *packet) :
	Hardware((const struct libswo_packet_hw *)packet)
{
}

PCValue::PCValue(const union libswo_packet *packet) :
	Hardware(packet)
{
}

uint8_t PCValue::get_comparator(void) const
{
	return _packet.pc_value.cmpn;
}

uint32_t PCValue::get_pc(void) const
{
	return _packet.pc_value.pc;
}

const std::string PCValue::to_string(void) const
{
	std::stringstream ss;

	ss << "Data trace PC value (comperator = ";
	ss << (unsigned int)get_comparator();
	ss << ", PC = " << std::hex << get_pc();
	ss << ", size = " << get_size() << " bytes)";

	return ss.str();
}

}
