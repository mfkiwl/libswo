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

AC_PREREQ([2.69])

AC_INIT([libswo], [0.1.0], [BUG-REPORT-ADDRESS])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Enable additional compiler warnings via -Wall and -Wextra. Use hidden
# visibility for all non-static symbols by default with -fvisibility=hidden.
C_CXX_FLAGS="-Wall -Wextra -Werror -fvisibility=hidden"
CFLAGS="$CFLAGS $C_CXX_FLAGS"
CXXFLAGS="$CXXFLAGS $C_CXX_FLAGS"

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Determine if a C++ compiler is available.
AC_CHECK_PROG([HAVE_CXX], [$CXX], [yes], [no])

# Automake >= 1.12 requires AM_PROG_AR when using options -Wall and -Werror.
# To be compatible with older versions of Automake use AM_PROG_AR if it's
# defined only. This line must occur before LT_INIT.
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

# Initialize libtool.
LT_INIT

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Disable progress and informational output of libtool.
AC_SUBST(AM_LIBTOOLFLAGS, '--silent')

AC_ARG_ENABLE(cxx, AS_HELP_STRING([--enable-cxx],
	[enable C++ bindings [default=no]]),
	[BINDINGS_CXX="$enableval"], [BINDINGS_CXX="no"])

if test "x$BINDINGS_CXX$HAVE_CXX" == "xyesno"; then
	AC_MSG_ERROR([No compiler for C++ bindings found.])
fi

AM_CONDITIONAL(BINDINGS_CXX, [test "x$BINDINGS_CXX" == "xyes"])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([libswo/Makefile])
AC_CONFIG_FILES([bindings/cxx/Makefile])
AC_CONFIG_FILES([bindings/cxx/libswocxx.pc])
AC_CONFIG_FILES([libswo.pc])

AC_OUTPUT
