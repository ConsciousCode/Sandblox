#ifndef SANDBLOX_MOD_GAME_H
#define SANDBLOX_MOD_GAME_H

#include <Python.h>

/**
 * Perform a hook on input.
**/
PyObject* perform_input_hook(PyObject*,PyObject*);
/**
 * Perform a hook on input.
**/
PyObject* remove_input_hook(PyObject*,PyObject*);

/**
 * Creates a set of input hook functions that allows the verification of the mod.
**/
PyObject* init_gamemod(Mod* m);

PyModuleDef game_mod={
	PyModuleDef_HEAD_INIT,
	"game",
	"Interface for global settings.",
	-1,
	NULL,NULL,NULL,NULL,NULL
};

#endif
