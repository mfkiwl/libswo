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

%module swopy

%{
#include "libswocxx.h"
%}

%include "stdint.i"
%include "std_string.i"
%include "std_vector.i"
%include "exception.i"
%include "typemaps.i"
%include "pybuffer.i"

%exception
{
        try {
                $action
        } catch (libswo::Error &error) {
                SWIG_exception(swig_exception_code(error.code), error.what());
        }
}

%{
static int swig_exception_code(int code)
{
	switch (code) {
	case LIBSWO_ERR:
		return SWIG_RuntimeError;
	case LIBSWO_ERR_MALLOC:
		return SWIG_MemoryError;
	case LIBSWO_ERR_ARG:
		return SWIG_ValueError;
	default:
		break;
	}

	return SWIG_RuntimeError;
}
%}

%pybuffer_binary(const uint8_t *data, size_t length)
void libswo::Context::feed(const uint8_t *data, size_t length);

/*
 * Map from std::vector<uint8_t> to Python bytes object for Python version 3
 * and to Python string object for Python version >= 2.6.
 */
%typemap(out) std::vector<uint8_t> {
	const char *dummy;
	const std::vector<uint8_t> &tmp = $1;

	dummy = reinterpret_cast<const char *>(&tmp[0]);
	$result = PyBytes_FromStringAndSize(dummy, tmp.size());
}

/*
 * Rename the Context class from the C++ bindings to a name with leading
 * underscore to prevent importing it from Python. To enable inheritance the
 * class is not ignored.
 */
%rename libswo::Context _Context;

%{
#include "Context.h"

Context::Context(size_t buffer_size) :
	libswo::Context(buffer_size)
{
	_py_args = NULL;
	_py_callback = NULL;
	_py_user_data = NULL;
}

Context::~Context(void)
{
	Py_XDECREF(_py_args);
	Py_XDECREF(_py_callback);
	Py_XDECREF(_py_user_data);
}

static PyObject *packet_object(const libswo::Packet &packet)
{
	PyObject *ret;
	void *tmp;

	tmp = (void *)&packet;

	switch (packet.get_type()) {
	case libswo::PACKET_TYPE_SYNC:
		ret = SWIG_NewPointerObj(tmp,
			SWIGTYPE_p_libswo__Synchronization, 0);
		break;
	case libswo::PACKET_TYPE_OVERFLOW:
		ret = SWIG_NewPointerObj(tmp, SWIGTYPE_p_libswo__Overflow, 0);
		break;
	case libswo::PACKET_TYPE_LTS:
		ret = SWIG_NewPointerObj(tmp,
			SWIGTYPE_p_libswo__LocalTimestamp, 0);
		break;
	case libswo::PACKET_TYPE_GTS1:
		ret = SWIG_NewPointerObj(tmp,
			SWIGTYPE_p_libswo__GlobalTimestamp1, 0);
		break;
	case libswo::PACKET_TYPE_GTS2:
		ret = SWIG_NewPointerObj(tmp,
			SWIGTYPE_p_libswo__GlobalTimestamp2, 0);
		break;
	case libswo::PACKET_TYPE_EXT:
		ret = SWIG_NewPointerObj(tmp, SWIGTYPE_p_libswo__Extension, 0);
		break;
	case libswo::PACKET_TYPE_INST:
		ret = SWIG_NewPointerObj(tmp,
			SWIGTYPE_p_libswo__Instrumentation, 0);
		break;
	case libswo::PACKET_TYPE_HW:
		ret = SWIG_NewPointerObj(tmp, SWIGTYPE_p_libswo__Hardware, 0);
		break;
	case libswo::PACKET_TYPE_UNKNOWN:
		ret = SWIG_NewPointerObj(tmp, SWIGTYPE_p_libswo__Unknown, 0);
		break;
	default:
		ret = NULL;
	}

	return ret;
}

static int packet_callback(const libswo::Packet &packet, void *user_data)
{
	int ret;
	PyObject *func;
	PyObject *res;
	PyObject *tmp;
	PyObject *obj;

	ret = PyArg_ParseTuple((PyObject *)user_data, "OO", &func, &tmp);

	if (!ret) {
		PyErr_Print();
		return LIBSWO_ERR;
	}

	obj = packet_object(packet);

	if (!obj) {
		PyErr_Format(PyExc_ValueError, "decoder callback invoked with "
			"invalid packet type: %i", packet.get_type());
		PyErr_Print();
		return LIBSWO_ERR;
	}

	res = PyObject_CallFunctionObjArgs(func, obj, tmp, NULL);
	Py_DECREF(obj);

	if (!res) {
		PyErr_Print();
		return LIBSWO_ERR;
	}

	if (res == Py_None || res == Py_True) {
		ret = 1;
	} else if (res == Py_False) {
		ret = 0;
	} else {
		PyErr_SetString(PyExc_TypeError, "decoder callback function "
			"neither returned a boolean value nor None");
		PyErr_Print();
		ret = LIBSWO_ERR;
	}

	Py_DECREF(res);

	return ret;
}

void Context::set_callback(PyObject *callback, PyObject *user_data)
{
	if (!user_data)
		user_data = Py_None;

	if (!PyCallable_Check(callback))
		throw libswo::Error(LIBSWO_ERR_ARG);

	Py_XDECREF(_py_args);
	Py_XDECREF(_py_callback);
	Py_XDECREF(_py_user_data);

	_py_callback = callback;
	Py_INCREF(_py_callback);

	_py_user_data = user_data;
	Py_INCREF(_py_user_data);

	_py_args = Py_BuildValue("(OO)", _py_callback, _py_user_data);
	libswo::Context::set_callback(&packet_callback, (void *)_py_args);
}
%}

/* Disable API visibility feature because SWIG does not work with it. */
#define LIBSWO_API
#define LIBSWO_PRIV

%include "libswocxx.h"
%include "Context.h"
