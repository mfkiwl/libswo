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

#include <sys/types.h>

#include "libswocxx.h"

namespace libswo
{

Context::Context(size_t buffer_size)
{
	int ret;

	ret = libswo_init(&_context, NULL, buffer_size);

	if (ret != LIBSWO_OK)
		throw Error(ret);
}

Context::Context(uint8_t *buffer, size_t buffer_size)
{
	int ret;

	ret = libswo_init(&_context, buffer, buffer_size);

	if (ret != LIBSWO_OK)
		throw Error(ret);
}

Context::~Context(void)
{
	libswo_exit(_context);
}

enum LogLevel Context::get_log_level(void) const
{
	int ret;

	ret = libswo_log_get_level(_context);

	if (ret < 0)
		throw Error(ret);

	return static_cast<enum LogLevel>(ret);
}

void Context::set_log_level(enum LogLevel level)
{
	int ret;

	ret = libswo_log_set_level(_context, level);

	if (ret != LIBSWO_OK)
		throw Error(ret);
}

string Context::get_log_domain(void) const
{
	const char *tmp;

	tmp = libswo_log_get_domain(_context);

	if (!tmp)
		throw Error(LIBSWO_ERR);

	return tmp;
}

void Context::set_log_domain(const string &domain)
{
	int ret;

	ret = libswo_log_set_domain(_context, domain.c_str());

	if (ret != LIBSWO_OK)
		throw Error(ret);
}

void Context::feed(const uint8_t *buffer, size_t length)
{
	int ret;

	ret = libswo_feed(_context, buffer, length);

	if (ret != LIBSWO_OK)
		throw Error(ret);
}

static int packet_callback(struct libswo_context *ctx,
		const union libswo_packet *packet, void *user_data)
{
	int ret;
	DecoderCallbackHelper *helper;
	Packet *tmp;

	(void)ctx;

	helper = (DecoderCallbackHelper *)user_data;

	switch (packet->type) {
	case LIBSWO_PACKET_TYPE_SYNC:
		tmp = new Synchronization(packet);
		break;
	case LIBSWO_PACKET_TYPE_OVERFLOW:
		tmp = new Overflow(packet);
		break;
	case LIBSWO_PACKET_TYPE_LTS:
		tmp = new LocalTimestamp(packet);
		break;
	case LIBSWO_PACKET_TYPE_GTS1:
		tmp = new GlobalTimestamp1(packet);
		break;
	case LIBSWO_PACKET_TYPE_GTS2:
		tmp = new GlobalTimestamp2(packet);
		break;
	case LIBSWO_PACKET_TYPE_EXT:
		tmp = new Extension(packet);
		break;
	case LIBSWO_PACKET_TYPE_INST:
		tmp = new Instrumentation(packet);
		break;
	case LIBSWO_PACKET_TYPE_HW:
		tmp = new Hardware(packet);
		break;
	case LIBSWO_PACKET_TYPE_UNKNOWN:
		tmp = new Unknown(packet);
		break;
	}

	ret = helper->callback(*tmp, helper->user_data);
	delete tmp;

	return ret;
}

size_t Context::decode(size_t limit, uint32_t flags)
{
	ssize_t ret;

	ret = libswo_decode(_context, limit, flags);

	if (ret < 0)
		throw Error(ret);

	return ret;
}

void Context::set_callback(DecoderCallback callback, void *user_data)
{
	int ret;

	if (callback) {
		_decoder_callback.callback = callback;
		_decoder_callback.user_data = user_data;
		ret = libswo_set_callback(_context, &packet_callback,
			&_decoder_callback);
	} else {
		ret = libswo_set_callback(_context, NULL, NULL);
	}

	if (ret != LIBSWO_OK)
		throw Error(ret);
}

}
