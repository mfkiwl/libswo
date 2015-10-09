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

LocalTimestamp::LocalTimestamp(const struct libswo_packet_lts *packet)
{
	_packet = *((const union libswo_packet *)packet);
}

LocalTimestamp::LocalTimestamp(const union libswo_packet *packet)
{
	_packet = *packet;
}

uint32_t LocalTimestamp::get_value(void) const
{
	return _packet.lts.value;
}

enum LocalTimestampRelation LocalTimestamp::get_relation(void) const
{
	return static_cast<enum LocalTimestampRelation>(_packet.lts.relation);
}

}
