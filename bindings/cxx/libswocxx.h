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

#ifndef LIBSWOCXX_H
#define LIBSWOCXX_H

#include <stdint.h>
#include <string>
#include <vector>
#include <stdexcept>

#include <libswo/libswo.h>

namespace libswo
{

using namespace std;

enum LogLevel {
	LOG_LEVEL_NONE = LIBSWO_LOG_LEVEL_NONE,
	LOG_LEVEL_ERROR = LIBSWO_LOG_LEVEL_ERROR,
	LOG_LEVEL_WARNING = LIBSWO_LOG_LEVEL_WARNING,
	LOG_LEVEL_INFO = LIBSWO_LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG = LIBSWO_LOG_LEVEL_DEBUG
};

enum PacketType {
	PACKET_TYPE_SYNC = LIBSWO_PACKET_TYPE_SYNC,
	PACKET_TYPE_OVERFLOW = LIBSWO_PACKET_TYPE_OVERFLOW,
	PACKET_TYPE_LTS = LIBSWO_PACKET_TYPE_LTS,
	PACKET_TYPE_GTS1 = LIBSWO_PACKET_TYPE_GTS1,
	PACKET_TYPE_GTS2 = LIBSWO_PACKET_TYPE_GTS2,
	PACKET_TYPE_EXT = LIBSWO_PACKET_TYPE_EXT,
	PACKET_TYPE_INST = LIBSWO_PACKET_TYPE_INST,
	PACKET_TYPE_HW = LIBSWO_PACKET_TYPE_HW,
	PACKET_TYPE_UNKNOWN = LIBSWO_PACKET_TYPE_UNKNOWN
};

enum LocalTimestampRelation {
	LTS_REL_SYNC = LIBSWO_LTS_REL_SYNC,
	LTS_REL_TS = LIBSWO_LTS_REL_TS,
	LTS_REL_SRC = LIBSWO_LTS_REL_SRC,
	LTS_REL_BOTH = LIBSWO_LTS_REL_BOTH
};

enum ExtensionSource {
	EXT_SRC_ITM = LIBSWO_EXT_SRC_ITM,
	EXT_SRC_HW = LIBSWO_EXT_SRC_HW
};

enum DecoderFlags {
	DF_EOS = LIBSWO_DF_EOS
};

class LIBSWO_API Error : public exception
{
public:
	Error(int error_code);
	~Error(void) throw();

	const char *what(void) const throw();
	const int code;
};

class LIBSWO_API Packet
{
public:
	virtual ~Packet(void) = 0;

	enum PacketType get_type(void) const;
	size_t get_size(void) const;
protected:
	union libswo_packet _packet;
};

typedef int (*DecoderCallback)(const Packet &packet, void *user_data);

class LIBSWO_API Synchronization : public Packet
{
public:
	Synchronization(const struct libswo_packet_sync *packet);
	Synchronization(const union libswo_packet *packet);
};

class LIBSWO_API Overflow : public Packet
{
public:
	Overflow(const struct libswo_packet_of *packet);
	Overflow(const union libswo_packet *packet);
};

class LIBSWO_API LocalTimestamp : public Packet
{
public:
	LocalTimestamp(const struct libswo_packet_lts *packet);
	LocalTimestamp(const union libswo_packet *packet);

	enum LocalTimestampRelation get_relation() const;
	uint32_t get_value(void) const;
};

class LIBSWO_API GlobalTimestamp1 : public Packet
{
public:
	GlobalTimestamp1(const struct libswo_packet_gts1 *packet);
	GlobalTimestamp1(const union libswo_packet *packet);

	uint32_t get_value(void) const;
	bool get_clkch(void) const;
	bool get_wrap(void) const;
};

class LIBSWO_API GlobalTimestamp2 : public Packet
{
public:
	GlobalTimestamp2(const struct libswo_packet_gts2 *packet);
	GlobalTimestamp2(const union libswo_packet *packet);

	uint32_t get_value(void) const;
};

class LIBSWO_API Extension : public Packet
{
public:
	Extension(const struct libswo_packet_ext *packet);
	Extension(const union libswo_packet *packet);

	enum ExtensionSource get_source(void) const;
	uint32_t get_value(void) const;
};

class LIBSWO_API Instrumentation : public Packet
{
public:
	Instrumentation(const struct libswo_packet_inst *packet);
	Instrumentation(const union libswo_packet *packet);

	uint8_t get_address(void) const;
	const vector<uint8_t> get_payload(void) const;
	uint32_t get_value(void) const;
};

class LIBSWO_API Hardware : public Packet
{
public:
	Hardware(const struct libswo_packet_hw *packet);
	Hardware(const union libswo_packet *packet);

	uint8_t get_address(void) const;
	const vector<uint8_t> get_payload(void) const;
	uint32_t get_value(void) const;
};

class LIBSWO_API Unknown : public Packet
{
public:
	Unknown(const struct libswo_packet_unknown *packet);
	Unknown(const union libswo_packet *packet);
};

class LIBSWO_PRIV DecoderCallbackHelper
{
public:
	DecoderCallback callback;
	void *user_data;
};

class LIBSWO_API Context
{
public:
	Context(size_t buffer_size);
	Context(uint8_t *buffer, size_t buffer_size);
	~Context(void);

	enum LogLevel get_log_level(void) const;
	void set_log_level(enum LogLevel level);

	string get_log_domain(void) const;
	void set_log_domain(const string &domain);

	void set_callback(DecoderCallback callback, void *user_data = NULL);

	void feed(const uint8_t *data, size_t length);
	void decode(uint32_t flags = 0);
private:
	struct libswo_context *_context;
	DecoderCallbackHelper _decoder_callback;
};

class LIBSWO_API Version {
public:
	static int get_package_major(void);
	static int get_package_minor(void);
	static int get_package_micro(void);
	static string get_package_string(void);

	static int get_lib_current(void);
	static int get_lib_revision(void);
	static int get_lib_age(void);
	static string get_lib_string(void);
};

}

#endif /* LIBSWOCXX_H */
