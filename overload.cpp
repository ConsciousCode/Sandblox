#include <Python.h>
#include <cstring>
#include <exception>

#include "overload.h"
#include "error.h"
#include "mod.h"
#include "mod/modwrapper.h"

/*
 * Hook functions
*/

///TODO: Implement support for libraries packaged in the mod.
PyObject* import_hook(PyObject* self,PyObject* args,PyObject* kwargs){
	PyObject* pyname;
	PyObject* nope;

	PyArg_ParseTuple(args,"sOOOO",&pyname,&nope,&nope,&nope,&nope);

	const char* name=PyBytes_AsString(pyname);

	for(unsigned i=0;whitelisted_libs[i]!=NULL;++i){
		if(strcmp(name,whitelisted_libs[i])){
			return PyObject_Call(original_import,args,kwargs);
		}
	}

	PyErr_Format(PyExc_ImportError,"No module named %s",name);

	return NULL;
}

PyObject* contextualize_import(Mod* m){
	return ModBinding_New(m,import_hook);
}

PyObject* get_globals(){
	PyObject* globals=PyEval_GetGlobals();

	if(globals!=NULL){
		Py_IncRef(globals);
		return globals;
	}
	else{
		return NULL;
	}
}

static PyObject* get_builtins(){
	PyObject* builtins_str=PyUnicode_InternFromString("__builtins__");
	PyObject* globals=get_globals();

	PyObject* builtins;

	std::string error;

	if(builtins_str==NULL){
		error="Unicode initialization failed obtaining builtin.";
		goto err;
	}

	if(globals!=NULL){
		Py_IncRef(globals);
		builtins=PyObject_GetItem(globals,builtins_str);
		if(builtins==NULL){
			error="__builtins__ not found in globals.";
			goto err;
		}
	}
	else{
		error="Unable to import builtins.";
		goto err;
	}

	return builtins;

	err:
		throw HookError(error);
}

void init_overload(){
	safe_builtins=get_builtins();
	original_import=PyObject_GetAttrString(safe_builtins,"__import__");

	for(unsigned i=0;blacklisted_builtins[i]!=NULL;++i){
		PyObject_DelAttrString(safe_builtins,blacklisted_builtins[i]);
	}
}
