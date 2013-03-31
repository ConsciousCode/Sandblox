#ifndef SANDBOX_MOD_MODWRAPPER_H
#define SANDBOX_MOD_MODWRAPPER_H

#include <Python.h>

/**
 * Returns a new ModBinding object.
 *
 * @param m The mod context.
 * @param func The function to bind.
**/
PyObject* ModBinding_New(Mod* m,ternaryfunc func);

PyObject* ModBinding_call(PyObject*,PyObject*,PyObject*);

/**
 * Binds a function to a mod for context.
**/
struct ModBinding{
	PyObject_HEAD

	/**
	 * The context of the callback.
	**/
	Mod* mod;

	/**
	 * The bound function.
	**/
	ternaryfunc callback;
};

static PyTypeObject ModBinding_Type={
	PyObject_HEAD_INIT(NULL)
	0,						 /*ob_size*/
	(char*)"ModBinding",			 /*tp_name*/
	sizeof(ModBinding),			 /*tp_basicsize*/
	0,						 /*tp_itemsize*/
	0,						 /*tp_dealloc*/
	0,						 /*tp_print*/
	0,						 /*tp_getattr*/
	0,						 /*tp_setattr*/
	0,						 /*tp_compare*/
	0,						 /*tp_repr*/
	0,						 /*tp_as_number*/
	0,						 /*tp_as_sequence*/
	0,						 /*tp_as_mapping*/
	0,						 /*tp_hash */
	(ternaryfunc)ModBinding_call,/*tp_call*/
	0,						 /*tp_str*/
	0,						 /*tp_getattro*/
	0,						 /*tp_setattro*/
	0,						 /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE, /*tp_flags*/
	(char*)"Wraps an inner function in a mod context.",/* tp_doc */
};

#endif
