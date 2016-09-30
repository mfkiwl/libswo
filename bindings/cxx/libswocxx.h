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
	PACKET_TYPE_UNKNOWN = LIBSWO_PACKET_TYPE_UNKNOWN,
	PACKET_TYPE_SYNC = LIBSWO_PACKET_TYPE_SYNC,
	PACKET_TYPE_OVERFLOW = LIBSWO_PACKET_TYPE_OVERFLOW,
	PACKET_TYPE_LTS = LIBSWO_PACKET_TYPE_LTS,
	PACKET_TYPE_GTS1 = LIBSWO_PACKET_TYPE_GTS1,
	PACKET_TYPE_GTS2 = LIBSWO_PACKET_TYPE_GTS2,
	PACKET_TYPE_EXT = LIBSWO_PACKET_TYPE_EXT,
	PACKET_TYPE_INST = LIBSWO_PACKET_TYPE_INST,
	PACKET_TYPE_HW = LIBSWO_PACKET_TYPE_HW,
	PACKET_TYPE_DWT_EVTCNT = LIBSWO_PACKET_TYPE_DWT_EVTCNT,
	PACKET_TYPE_DWT_EXCTRACE = LIBSWO_PACKET_TYPE_DWT_EXCTRACE,
	PACKET_TYPE_DWT_PC_SAMPLE = LIBSWO_PACKET_TYPE_DWT_PC_SAMPLE,
	PACKET_TYPE_DWT_PC_VALUE = LIBSWO_PACKET_TYPE_DWT_PC_VALUE,
	PACKET_TYPE_DWT_ADDR_OFFSET = LIBSWO_PACKET_TYPE_DWT_ADDR_OFFSET,
	PACKET_TYPE_DWT_DATA_VALUE = LIBSWO_PACKET_TYPE_DWT_DATA_VALUE
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

enum ExceptionTraceFunction {
	EXCTRACE_FUNC_RESERVED = LIBSWO_EXCTRACE_FUNC_RESERVED,
	EXCTRACE_FUNC_ENTER = LIBSWO_EXCTRACE_FUNC_ENTER,
	EXCTRACE_FUNC_EXIT = LIBSWO_EXCTRACE_FUNC_EXIT,
	EXCTRACE_FUNC_RETURN = LIBSWO_EXCTRACE_FUNC_RETURN
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
	virtual const string to_string(void) const = 0;
protected:
	union libswo_packet _packet;
};

class LIBSWO_API PayloadPacket : public Packet
{
public:
	virtual ~PayloadPacket(void) = 0;

	const vector<uint8_t> get_data(void) const;
};

typedef int (*DecoderCallback)(const Packet &packet, void *user_data);
typedef int (*LogCallback)(enum LogLevel level, const std::string &message,
		void *user_data);

class LIBSWO_API Unknown : public PayloadPacket
{
public:
	Unknown(const struct libswo_packet_unknown *packet);
	Unknown(const union libswo_packet *packet);

	const string to_string(void) const;
};

class LIBSWO_API Synchronization : public Packet
{
public:
	Synchronization(const struct libswo_packet_sync *packet);
	Synchronization(const union libswo_packet *packet);

	const string to_string(void) const;
};

class LIBSWO_API Overflow : public PayloadPacket
{
public:
	Overflow(const struct libswo_packet_of *packet);
	Overflow(const union libswo_packet *packet);

	const string to_string(void) const;
};

class LIBSWO_API LocalTimestamp : public PayloadPacket
{
public:
	LocalTimestamp(const struct libswo_packet_lts *packet);
	LocalTimestamp(const union libswo_packet *packet);

	enum LocalTimestampRelation get_relation() const;
	uint32_t get_value(void) const;

	const string to_string(void) const;
};

class LIBSWO_API GlobalTimestamp1 : public PayloadPacket
{
public:
	GlobalTimestamp1(const struct libswo_packet_gts1 *packet);
	GlobalTimestamp1(const union libswo_packet *packet);

	uint32_t get_value(void) const;
	bool get_clkch(void) const;
	bool get_wrap(void) const;

	const string to_string(void) const;
};

class LIBSWO_API GlobalTimestamp2 : public PayloadPacket
{
public:
	GlobalTimestamp2(const struct libswo_packet_gts2 *packet);
	GlobalTimestamp2(const union libswo_packet *packet);

	uint32_t get_value(void) const;

	const string to_string(void) const;
};

class LIBSWO_API Extension : public PayloadPacket
{
public:
	Extension(const struct libswo_packet_ext *packet);
	Extension(const union libswo_packet *packet);

	enum ExtensionSource get_source(void) const;
	uint32_t get_value(void) const;

	const string to_string(void) const;
};

class LIBSWO_API Instrumentation : public PayloadPacket
{
public:
	Instrumentation(const struct libswo_packet_inst *packet);
	Instrumentation(const union libswo_packet *packet);

	uint8_t get_address(void) const;
	const vector<uint8_t> get_payload(void) const;
	uint32_t get_value(void) const;

	const string to_string(void) const;
};

class LIBSWO_API Hardware : public PayloadPacket
{
public:
	Hardware(const struct libswo_packet_hw *packet);
	Hardware(const union libswo_packet *packet);

	uint8_t get_address(void) const;
	const vector<uint8_t> get_payload(void) const;
	uint32_t get_value(void) const;

	const string to_string(void) const;
};

class LIBSWO_API EventCounter : public Hardware
{
public:
	EventCounter(const struct libswo_packet_dwt_evtcnt *packet);
	EventCounter(const union libswo_packet *packet);

	bool get_cpi(void) const;
	bool get_exc(void) const;
	bool get_sleep(void) const;
	bool get_lsu(void) const;
	bool get_fold(void) const;
	bool get_cyc(void) const;

	const string to_string(void) const;
};

class LIBSWO_API ExceptionTrace : public Hardware
{
public:
	ExceptionTrace(const struct libswo_packet_dwt_exctrace *packet);
	ExceptionTrace(const union libswo_packet *packet);

	uint16_t get_exception(void) const;
	enum ExceptionTraceFunction get_function(void) const;

	const string to_string(void) const;
};

class LIBSWO_API PCSample : public Hardware
{
public:
	PCSample(const struct libswo_packet_dwt_pc_sample *packet);
	PCSample(const union libswo_packet *packet);

	bool get_sleep(void) const;
	uint32_t get_pc(void) const;

	const string to_string(void) const;
};

class LIBSWO_API PCValue : public Hardware
{
public:
	PCValue(const struct libswo_packet_dwt_pc_value *packet);
	PCValue(const union libswo_packet *packet);

	uint8_t get_comparator(void) const;
	uint32_t get_pc(void) const;

	const string to_string(void) const;
};

class LIBSWO_API AddressOffset : public Hardware
{
public:
	AddressOffset(const struct libswo_packet_dwt_addr_offset *packet);
	AddressOffset(const union libswo_packet *packet);

	uint8_t get_comparator(void) const;
	uint16_t get_offset(void) const;

	const string to_string(void) const;
};

class LIBSWO_API DataValue : public Hardware
{
public:
	DataValue(const struct libswo_packet_dwt_data_value *packet);
	DataValue(const union libswo_packet *packet);

	bool get_wnr(void) const;
	uint8_t get_comparator(void) const;
	uint32_t get_data_value(void) const;

	const string to_string(void) const;
};

class LIBSWO_PRIV DecoderCallbackHelper
{
public:
	DecoderCallback callback;
	void *user_data;
};

class LIBSWO_PRIV LogCallbackHelper
{
public:
	LogCallback callback;
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
	void set_log_callback(LogCallback callback, void *user_data = NULL);

	void set_callback(DecoderCallback callback, void *user_data = NULL);

	void feed(const uint8_t *data, size_t length);
	void decode(uint32_t flags = 0);
private:
	struct libswo_context *_context;
	DecoderCallbackHelper _decoder_callback;
	LogCallbackHelper _log_callback;
};

class LIBSWO_API Version {
public:
	static int get_package_major(void);
	static int get_package_minor(void);
	static int get_package_micro(void);
	static string get_package_string(void);

	static int get_library_current(void);
	static int get_library_revision(void);
	static int get_library_age(void);
	static string get_library_string(void);
};

}

#endif /* LIBSWOCXX_H */
