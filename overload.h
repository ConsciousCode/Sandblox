/**
 * Overload the built-in import function to restrict access.
**/
#ifndef SANDBLOX_IMPORT_H
#define SANDBLOX_IMPORT_H

#include <Python.h>
#include <stdexcept>

/**
 * __import__ library whitelist for mods.
**/
const char* whitelisted_libs[]={
	//Specialized libraries(may or may not be initialized for the mod)
	"sandblox",//Access to global SANDBLOX settings
	"game",//Access to the game(map data,relevant user statistics,user input,etc)
	"mod",//Access to the mod's own settings and resources
	"render",//Direct access to rendering within the environment

	//Standard Python 3.3 libraries
	"array",//Note: Overloading should be looked into to avoid extensive allocations
	"base64",
	"binascii",
	"bisect",
	"calendar",
	"cmath",
	"collections",
	"colorsys",
	"copy",
	"datetime",
	"fractions",
	"hashlib",
	"hmac",
	"heapq",
	"json",
	//"marshal" //Note: Consider having the interpreter handle marshalling objects over internet connections
	"math",
	"operator",
	"random",
	"re",//Note: Watch this carefully for regex attacks
	"shlex",
	"string",
	"struct",
	"time",
	"timeit",
	"types",
	"unicodedata",
	"uuid",
	NULL
};

const char* blacklisted_builtins[]={
	"print",	//interrupts game flow
	"compile",	//might be able to work around security
	"exec",		//same as compile()
	"eval",		//same as compile()
	"exit",		//a mod shouldn't be able to stop the game
	"input",	//breach of security, having direct user input
	"help",		//why would a mod need this?
	"open",		//file access is a security breach
	"quit",		//same as exit()
	NULL
};

/**
 * Original import function.
**/
PyObject* original_import;

/**
 * A proxy function wrapping the standard __import__.
**/
PyObject* import_hook(PyObject*,PyObject*,PyObject*);

/**
 * Returns a proxy function wrapping import_hook in the context of the given mod.
 *
 * @param m The mod context.
**/
PyObject* contextualize_import(Mod* m);

/**
 * Initialize global overloads (used by all mods)
**/
void init_overload();

/**
 * All the safe builtins as dictated by the blacklist.
**/
PyObject* safe_builtins;

#endif
