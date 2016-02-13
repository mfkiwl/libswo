##
## This file is part of the libswo project.
##
## Copyright (C) 2014-2015 Marc Schink <swo-dev@marcschink.de>
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

lib_LTLIBRARIES = libswo.la

libswo_la_SOURCES = \
	buffer.c \
	core.c \
	decoder.c \
	error.c \
	log.c \
	version.c

libswo_la_LDFLAGS = $(LIBSWO_LIB_LDFLAGS) -no-undefined

library_includedir = $(includedir)/libswo
library_include_HEADERS = \
	libswo.h \
	version.h

noinst_HEADERS = libswo-internal.h
