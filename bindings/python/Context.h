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

#ifndef LIBSWO_BINDINGS_PYTHON_CONTEXT_H
#define LIBSWO_BINDINGS_PYTHON_CONTEXT_H

#include <string>

#include "libswocxx.h"

class Context : public libswo::Context
{
public:
	Context(size_t buffer_size);
	~Context(void);

	void set_callback(PyObject *pyfunc, PyObject *user_data = NULL);

	void feed(std::string data);
private:
	PyObject *_py_args;
	PyObject *_py_callback;
	PyObject *_py_user_data;
};

#endif /* LIBSWO_BINDINGS_PYTHON_CONTEXT_H */
