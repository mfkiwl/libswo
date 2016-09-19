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

ExceptionTrace::ExceptionTrace(
		const struct libswo_packet_dwt_exctrace *packet) :
	Hardware((const struct libswo_packet_hw *)packet)
{
}

ExceptionTrace::ExceptionTrace(const union libswo_packet *packet) :
	Hardware(packet)
{
}

uint16_t ExceptionTrace::get_exception(void) const
{
	return _packet.exctrace.exception;
}

enum ExceptionTraceFunction ExceptionTrace::get_function(void) const
{
	return static_cast<ExceptionTraceFunction>(_packet.exctrace.function);
}

const std::string ExceptionTrace::to_string(void) const
{
	std::stringstream ss;

	ss << "Exception trace (exception = " << get_exception();
	ss << ", function = ";

	switch (get_function()) {
	case EXCTRACE_FUNC_ENTER:
		ss << "enter";
		break;
	case EXCTRACE_FUNC_EXIT:
		ss << "exit";
		break;
	case EXCTRACE_FUNC_RETURN:
		ss << "return";
		break;
	default:
		ss << "reserved";
	}

	ss << ", size = " << get_size() << " bytes)";

	return ss.str();
}

}
