#ifndef SANDBLOX_VERSION_H
#define SANDBLOX_VERSION_H

#include <Python.h>
#include <structmember.h>

/**
 * Represents a version, used in the mod and sandblox modules.
**/
struct Version{
	PyObject_HEAD

	unsigned char major;
	unsigned char minor;
	unsigned char build;
	unsigned char revision;
};

int version_init(PyObject*,PyObject*);
PyObject* version_repr(PyObject*,PyObject*);
PyObject* version_str(PyObject*,PyObject*);

/**
 * Convert a version string to a packed integer.
 *
 * @param s The version string (format: byte[.byte[.byte[.byte]]], ex: "1.0.4.37")
 *
 * @return A packed integer representing the version.
**/
unsigned s2version(const std::string& s);

/**
 * Make a new Version instance.
 *
 * @param s - The properly formatted string containing the version.
**/
PyObject* Version_New(const char* s);

/**
 * Make a new Version instance
 *
 * @param v - A packed integer containing the version.
**/
PyObject* Version_New(unsigned v);

static PyMemberDef Version_members[]={
	{(char*)"major",T_UINT,offsetof(Version,major),0,(char*)"Major version"},
	{(char*)"minor",T_UINT,offsetof(Version,minor),0,(char*)"Minor version"},
	{(char*)"build",T_UINT,offsetof(Version,build),0,(char*)"Build version"},
	{(char*)"revision",T_UINT,offsetof(Version,build),0,(char*)"Revision version"}
};

static PyTypeObject VersionType={
	PyVarObject_HEAD_INIT(NULL,0)
	(char*)"version",			 /* tp_name */
	sizeof(Version),			 /* tp_basicsize */
	0,						 /* tp_itemsize */
	0,						 /* tp_dealloc */
	0,						 /* tp_print */
	0,						 /* tp_getattr */
	0,						 /* tp_setattr */
	0,						 /* tp_reserved */
	(reprfunc)version_repr,	 /* tp_repr */
	0,						 /* tp_as_number */
	0,						 /* tp_as_sequence */
	0,						 /* tp_as_mapping */
	0,						 /* tp_hash  */
	0,						 /* tp_call */
	(reprfunc)version_str,	 /* tp_str */
	0,						 /* tp_getattro */
	0,						 /* tp_setattro */
	0,						 /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT |
		Py_TPFLAGS_BASETYPE,   /* tp_flags */
	(char*)"A standard version descriptor.",		   /* tp_doc */
	0,						 /* tp_traverse */
	0,						 /* tp_clear */
	0,						 /* tp_richcompare */
	0,						 /* tp_weaklistoffset */
	0,						 /* tp_iter */
	0,						 /* tp_iternext */
	0,						 /* tp_methods */
	Version_members,		 /* tp_members */
	0,						 /* tp_getset */
	0,						 /* tp_base */
	0,						 /* tp_dict */
	0,						 /* tp_descr_get */
	0,						 /* tp_descr_set */
	0,						 /* tp_dictoffset */
	(initproc)version_init,	  /* tp_init */
	0,						 /* tp_alloc */
	0						 /* tp_new */
};

#endif
