#ifndef SANDBLOX_MOD_SANDBLOX_H
#define SANDBLOX_MOD_SANDBLOX_H

/**
 * Initialize the sandblox module.
**/
PyObject* init_sandbloxmod();

/**
 * Methods in the sandblox mod.
**/
static PyMethodDef sandblox_methods[]={
	{NULL}
};

/**
 * A definition of the sandblox mod.
**/
PyModuleDef Sandblox_mod={
	PyModuleDef_HEAD_INIT,
	"sandblox",
	"Interface for global settings.",
	-1,
	sandblox_methods,
	NULL,NULL,NULL,NULL
};

#endif
