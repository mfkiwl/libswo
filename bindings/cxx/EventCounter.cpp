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

EventCounter::EventCounter(const struct libswo_packet_dwt_evtcnt *packet) :
	Hardware((const struct libswo_packet_hw *)packet)
{
}

EventCounter::EventCounter(const union libswo_packet *packet) :
	Hardware(packet)
{
}

bool EventCounter::get_cpi(void) const
{
	return _packet.evtcnt.cpi;
}

bool EventCounter::get_exc(void) const
{
	return _packet.evtcnt.exc;
}

bool EventCounter::get_sleep(void) const
{
	return _packet.evtcnt.sleep;
}

bool EventCounter::get_lsu(void) const
{
	return _packet.evtcnt.lsu;
}

bool EventCounter::get_fold(void) const
{
	return _packet.evtcnt.fold;
}

bool EventCounter::get_cyc(void) const
{
	return _packet.evtcnt.cyc;
}

const std::string EventCounter::to_string(void) const
{
	std::stringstream ss;

	ss << "Event counter (CPI = " << get_cpi();
	ss << ", exc = " << get_exc();
	ss << ", sleep = " << get_sleep();
	ss << ", LSU = " << get_lsu();
	ss << ", fold = " << get_fold();
	ss << ", cyc = " << get_cyc();
	ss << ", size = " << get_size() << " bytes)";

	return ss.str();
}

}
