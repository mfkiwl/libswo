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

CXX_SRCDIR = $(top_srcdir)/bindings/cxx
CXX_BUILDDIR = $(top_builddir)/bindings/cxx

EXTRA_DIST = swopy.i __init__.py Context.h

all-local: swopy build-lock

# Generate Python package structure.
swopy:
	mkdir -p swopy
	cp $(srcdir)/__init__.py swopy

# Use empty target build-lock to ensure that the Python module is built only if
# one of its source files has changed since the last invocation.
build-lock: setup.py $(srcdir)/swopy.i Context.h $(CXX_BUILDDIR)/libswocxx.la
	$(SWIG) -python -c++ -Wall -outdir swopy -outcurrentdir \
		-I$(CXX_SRCDIR) $(srcdir)/swopy.i
	$(PYTHON) setup.py --quiet build
	touch build-lock

install-exec-local:
	$(PYTHON) setup.py --quiet install --prefix $(prefix)

clean-local:
	$(PYTHON) setup.py --quiet clean --all
	rm -f swopy_wrap.cxx
	rm -f build-lock
	rm -rf build
	rm -rf swopy
