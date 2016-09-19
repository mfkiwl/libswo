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

PCSample::PCSample(const struct libswo_packet_dwt_pc_sample *packet) :
	Hardware((const struct libswo_packet_hw *)packet)
{
}

PCSample::PCSample(const union libswo_packet *packet) :
	Hardware(packet)
{
}

bool PCSample::get_sleep(void) const
{
	return _packet.pc_sample.sleep;
}

uint32_t PCSample::get_pc(void) const
{
	return _packet.pc_sample.pc;
}

const std::string PCSample::to_string(void) const
{
	std::stringstream ss;

	if (get_sleep()) {
		ss << "Periodic PC sleep (";
	} else {
		ss << "Periodic PC sample (PC = ";
		ss << std::hex << get_pc() << ", ";
	}

	ss << "size = " << get_size() << " bytes)";

	return ss.str();
}

}
