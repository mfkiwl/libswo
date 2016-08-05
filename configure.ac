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

# Package version of libswo. This is not the same as the libtool interface
# version.
m4_define([libswo_version_package_major], [0])
m4_define([libswo_version_package_minor], [1])
m4_define([libswo_version_package_micro], [0])
m4_define([libswo_version_package], [libswo_version_package_major.libswo_version_package_minor.libswo_version_package_micro])

AC_INIT([libswo], [libswo_version_package], [BUG-REPORT-ADDRESS])
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

# Libtool interface version of libswo. This is not the same as the package
# version. For information about the versioning system of libtool, see:
# http://www.gnu.org/software/libtool/manual/libtool.html#Libtool-versioning
LIBSWO_VERSION_LIB_CURRENT=0
LIBSWO_VERSION_LIB_REVISION=0
LIBSWO_VERSION_LIB_AGE=0
LIBSWO_VERSION_LIB="$LIBSWO_VERSION_LIB_CURRENT:$LIBSWO_VERSION_LIB_REVISION:$LIBSWO_VERSION_LIB_AGE"

AC_SUBST(LIBSWO_VERSION_LIB_CURRENT)
AC_SUBST(LIBSWO_VERSION_LIB_REVISION)
AC_SUBST(LIBSWO_VERSION_LIB_AGE)
AC_SUBST(LIBSWO_VERSION_LIB)

LIBSWO_LIB_LDFLAGS="-version-info $LIBSWO_VERSION_LIB"

AC_SUBST([LIBSWO_LIB_LDFLAGS])

AC_ARG_ENABLE(cxx, AS_HELP_STRING([--enable-cxx],
	[enable C++ bindings [default=no]]),
	[BINDINGS_CXX="$enableval"], [BINDINGS_CXX="no"])

AC_ARG_ENABLE([python], AS_HELP_STRING([--enable-python],
	[enable Python bindings [default=no]]),
	[BINDINGS_PYTHON="$enableval"], [BINDINGS_PYTHON="no"])

cxx_msg=""
python_msg=""

if test "x$HAVE_PYMOD_SETUPTOOLS" = "xno"; then
	python_msg="Python setuptools required"
	BINDINGS_PYTHON="no"
fi

if test "x$SWIG" = "x"; then
	python_msg="SWIG required"
	BINDINGS_PYTHON="no"
fi

if test "x$HAVE_PYTHON_DEV" = "xno"; then
	python_msg="Python development files required"
	BINDINGS_PYTHON="no"
fi

if test "x$HAVE_PYTHON" = "xno"; then
	python_msg="Python interpreter required"
	BINDINGS_PYTHON="no"
fi

# Enable C++ bindings if not already enabled as they are required for Python
# bindings.
if test "x$BINDINGS_PYTHON$BINDINGS_CXX" = "xyesno"; then
	cxx_msg="enabled for Python bindings"
	BINDINGS_CXX="yes";
fi

if test "x$BINDINGS_CXX$HAVE_CXX" = "xyesno"; then
	cxx_msg="C++ compiler required"
	BINDINGS_CXX="no"
fi

if test "x$BINDINGS_PYTHON$BINDINGS_CXX" = "xyesno"; then
	python_msg="C++ bindings required"
	BINDINGS_PYTHON="no"
fi

AM_CONDITIONAL(BINDINGS_CXX, [test "x$BINDINGS_CXX" = "xyes"])
AM_CONDITIONAL(BINDINGS_PYTHON, [test "x$BINDINGS_PYTHON" = "xyes"])

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

LIBSWO_VERSION_PACKAGE_MAJOR=libswo_version_package_major
LIBSWO_VERSION_PACKAGE_MINOR=libswo_version_package_minor
LIBSWO_VERSION_PACKAGE_MICRO=libswo_version_package_micro
LIBSWO_VERSION_PACKAGE=libswo_version_package

AC_SUBST(LIBSWO_VERSION_PACKAGE_MAJOR)
AC_SUBST(LIBSWO_VERSION_PACKAGE_MINOR)
AC_SUBST(LIBSWO_VERSION_PACKAGE_MICRO)
AC_SUBST(LIBSWO_VERSION_PACKAGE)

# Use C99 compatible stdio functions on MinGW instead of the incompatible
# functions provided by Microsoft.
AS_CASE([$host_os], [mingw*],
	[AC_DEFINE([__USE_MINGW_ANSI_STDIO], [1],
		[Define to 1 to use C99 compatible stdio functions on MinGW.])])

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
echo " - Library version ................ $LIBSWO_VERSION_LIB"
echo " - Installation prefix ............ $prefix"
echo " - Building on .................... $build"
echo " - Building for ................... $host"

echo
echo "Enabled language bindings:"
echo " - C++ ............................ $BINDINGS_CXX$cxx_msg"
echo " - Python ......................... $BINDINGS_PYTHON$python_msg"
echo
