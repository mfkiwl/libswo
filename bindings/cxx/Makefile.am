##
## This file is part of the libswo project.
##
## Copyright (C) 2015 Marc Schink <swo-dev@marcschink.de>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

lib_LTLIBRARIES = libswocxx.la

libswocxx_la_SOURCES = \
	AddressOffset.cpp \
	Context.cpp \
	DataValue.cpp \
	Error.cpp \
	EventCounter.cpp \
	ExceptionTrace.cpp \
	Extension.cpp \
	GlobalTimestamp1.cpp \
	GlobalTimestamp2.cpp \
	Hardware.cpp \
	Instrumentation.cpp \
	LocalTimestamp.cpp \
	Overflow.cpp \
	Packet.cpp \
	PayloadPacket.cpp \
	PCSample.cpp \
	PCValue.cpp \
	Synchronization.cpp \
	Unknown.cpp \
	Version.cpp

libswocxx_la_LIBADD = $(top_builddir)/libswo/libswo.la
libswocxx_la_LDFLAGS = $(LIBSWO_LIB_LDFLAGS) -no-undefined
libswocxx_la_CXXFLAGS = -I$(top_srcdir) -I$(top_builddir)/libswo

library_includedir = $(includedir)/libswocxx
library_include_HEADERS = libswocxx.h
