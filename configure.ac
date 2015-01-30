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

AC_CANONICAL_HOST

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

# Check for SWIG.
AC_CHECK_PROG([HAVE_SWIG], [swig], [yes])

# Check for Python interpreter.
AM_PATH_PYTHON([2.7], [HAVE_PYTHON="yes"], [HAVE_PYTHON="no"])

# Check for Python development files.
PKG_CHECK_MODULES([python27], [python-2.7 >= 2.7],
	[HAVE_PYTHON_DEV="yes"], [HAVE_PYTHON_DEV="no"])

# Check for Python setuptools.
AX_PYTHON_MODULE([setuptools])

LIBSWO_LIB_LDFLAGS=""

case $host_os in
mingw*)
	LIBSWO_LIB_LDFLAGS+="-no-undefined"
esac

AC_SUBST([LIBSWO_LIB_LDFLAGS])

AC_ARG_ENABLE(cxx, AS_HELP_STRING([--enable-cxx],
	[enable C++ bindings [default=no]]),
	[BINDINGS_CXX="$enableval"], [BINDINGS_CXX="no"])

AC_ARG_ENABLE([python], AS_HELP_STRING([--enable-python],
	[enable Python bindings [default=no]]),
	[BINDINGS_PYTHON="$enableval"], [BINDINGS_PYTHON="no"])

cxx_msg=""
python_msg=""

if test "x$HAVE_PYMOD_SETUPTOOLS" == "xno"; then
	python_msg="Python setuptools required"
	BINDINGS_PYTHON="no"
fi

if test "x$HAVE_SWIG" != "xyes"; then
	python_msg="SWIG required"
	BINDINGS_PYTHON="no"
fi

if test "x$HAVE_PYTHON_DEV" == "xno"; then
	python_msg="Python development files required"
	BINDINGS_PYTHON="no"
fi

if test "x$HAVE_PYTHON" == "xno"; then
	python_msg="Python interpreter required"
	BINDINGS_PYTHON="no"
fi

# Enable C++ bindings if not already enabled as they are required for Python
# bindings.
if test "x$BINDINGS_PYTHON$BINDINGS_CXX" == "xyesno"; then
	cxx_msg="enabled for Python bindings"
	BINDINGS_CXX="yes";
fi

if test "x$BINDINGS_CXX$HAVE_CXX" == "xyesno"; then
	cxx_msg="C++ compiler required"
	BINDINGS_CXX="no"
fi

if test "x$BINDINGS_PYTHON$BINDINGS_CXX" == "xyesno"; then
	python_msg="C++ bindings required"
	BINDINGS_PYTHON="no"
fi

AM_CONDITIONAL(BINDINGS_CXX, [test "x$BINDINGS_CXX" == "xyes"])
AM_CONDITIONAL(BINDINGS_PYTHON, [test "x$BINDINGS_PYTHON" == "xyes"])

if test "x$cxx_msg" != "x"; then
	cxx_msg=" ($cxx_msg)"
elif test "x$enable_cxx" != "xyes"; then
	cxx_msg=" (disabled)"
fi

if test "x$enable_python" != "xyes"; then
	python_msg=" (disabled)"
elif test "x$python_msg" != "x"; then
	python_msg=" ($python_msg)"
fi

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([libswo/Makefile])
AC_CONFIG_FILES([bindings/cxx/Makefile])
AC_CONFIG_FILES([bindings/cxx/libswocxx.pc])
AC_CONFIG_FILES([bindings/python/Makefile])
AC_CONFIG_FILES([bindings/python/setup.py])
AC_CONFIG_FILES([libswo.pc])

AC_OUTPUT

echo
echo "Enabled language bindings:"
echo " - C++ ............................ $BINDINGS_CXX$cxx_msg"
echo " - Python ......................... $BINDINGS_PYTHON$python_msg"
