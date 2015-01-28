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

GlobalTimestamp1::GlobalTimestamp1(const struct libswo_packet_gts1 *packet)
{
	_packet = *((const union libswo_packet *)packet);
}

GlobalTimestamp1::GlobalTimestamp1(const union libswo_packet *packet)
{
	_packet = *packet;
}

size_t GlobalTimestamp1::get_size(void) const
{
	return _packet.gts1.size;
}

uint32_t GlobalTimestamp1::get_value(void) const
{
	return _packet.gts1.value;
}

bool GlobalTimestamp1::get_clkch(void) const
{
	return _packet.gts1.clkch;
}

bool GlobalTimestamp1::get_wrap(void) const
{
	return _packet.gts1.wrap;
}

}
