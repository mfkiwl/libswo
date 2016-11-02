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
LIBSWO_CFLAGS="$C_CXX_FLAGS"
LIBSWO_CXXFLAGS="$C_CXX_FLAGS"

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

# Initialize pkg-config.
PKG_PROG_PKG_CONFIG

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Disable progress and informational output of libtool.
AC_SUBST(AM_LIBTOOLFLAGS, '--silent')

# Check for SWIG.
AC_CHECK_PROGS([SWIG], [swig swig3.0 swig2.0])

# Check for Python interpreter.
AM_PATH_PYTHON([2.7], [HAVE_PYTHON="yes"], [HAVE_PYTHON="no"])

# Check for Python development files.
AC_MSG_CHECKING([for Python development files])

AS_IF([test -x "$(command -v $PYTHON-config)"], [HAVE_PYTHON_DEV="yes"],
	[HAVE_PYTHON_DEV="no"])

AC_MSG_RESULT([$HAVE_PYTHON_DEV])

# Check for Python setuptools.
AX_PYTHON_MODULE([setuptools])

LIBSWO_SET_PACKAGE_VERSION([LIBSWO_VERSION_PACKAGE], [AC_PACKAGE_VERSION])

# Libtool interface version of libswo. This is not the same as the package
# version. For information about the versioning system of libtool, see:
# http://www.gnu.org/software/libtool/manual/libtool.html#Libtool-versioning
LIBSWO_SET_LIBRARY_VERSION([LIBSWO_VERSION_LIBRARY], [0:0:0])

LIBSWO_LDFLAGS="-version-info $LIBSWO_VERSION_LIBRARY"

AC_ARG_ENABLE(cxx, AS_HELP_STRING([--enable-cxx],
	[enable C++ bindings [default=yes]]),
	[], [enable_cxx="yes"])

AC_ARG_ENABLE([python], AS_HELP_STRING([--enable-python],
	[enable Python bindings [default=yes]]),
	[], [enable_python="yes"])

if test "x$enable_cxx" != "xno"; then
	enable_cxx="yes"
fi

if test "x$enable_python" != "xno"; then
	enable_python="yes"
fi

cxx_msg=""
python_msg=""

if test "x$HAVE_CXX" = "xno"; then
	cxx_msg="C++ compiler required"
fi

if test "x$enable_cxx" = "xno"; then
	cxx_msg="disabled"
fi

if test -z "$cxx_msg"; then
	BINDINGS_CXX=$enable_cxx
else
	BINDINGS_CXX="no"
	cxx_msg=" ($cxx_msg)"
fi

if test "x$HAVE_PYMOD_SETUPTOOLS" = "xno"; then
	python_msg="Python setuptools required"
fi

if test "x$SWIG" = "x"; then
	python_msg="SWIG required"
fi

if test "x$HAVE_PYTHON_DEV" = "xno"; then
	python_msg="Python development files required"
fi

if test "x$HAVE_PYTHON" = "xno"; then
	python_msg="Python interpreter required"
fi

if test "x$enable_python$BINDINGS_CXX" = "xyesno"; then
	python_msg="C++ bindings required"
fi

if test "x$enable_python" = "xno"; then
	python_msg="disabled"
fi

if test -z "$python_msg"; then
	BINDINGS_PYTHON=$enable_python
else
	BINDINGS_PYTHON="no"
	python_msg=" ($python_msg)"
fi

AM_CONDITIONAL(BINDINGS_CXX, [test "x$BINDINGS_CXX" = "xyes"])
AM_CONDITIONAL(BINDINGS_PYTHON, [test "x$BINDINGS_PYTHON" = "xyes"])

# Use C99 compatible stdio functions on MinGW instead of the incompatible
# functions provided by Microsoft.
AS_CASE([$host_os], [mingw*],
	[AC_DEFINE([__USE_MINGW_ANSI_STDIO], [1],
		[Define to 1 to use C99 compatible stdio functions on MinGW.])])

AC_SUBST([LIBSWO_CFLAGS])
AC_SUBST([LIBSWO_CXXFLAGS])
AC_SUBST([LIBSWO_LDFLAGS])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([libswo/Makefile])
AC_CONFIG_FILES([libswo/version.h])
AC_CONFIG_FILES([bindings/cxx/Makefile])
AC_CONFIG_FILES([bindings/cxx/libswocxx.pc])
AC_CONFIG_FILES([bindings/python/Makefile])
AC_CONFIG_FILES([bindings/python/setup.py])
AC_CONFIG_FILES([libswo.pc])

AC_OUTPUT

echo
echo "libswo configuration summary:"
echo " - Package version ................ $LIBSWO_VERSION_PACKAGE"
echo " - Library version ................ $LIBSWO_VERSION_LIBRARY"
echo " - Installation prefix ............ $prefix"
echo " - Building on .................... $build"
echo " - Building for ................... $host"

echo
echo "Enabled language bindings:"
echo " - C++ ............................ $BINDINGS_CXX$cxx_msg"
echo " - Python ......................... $BINDINGS_PYTHON$python_msg"
echo
