#include <Python.h>
#include <cstring>
#include <exception>

#include "overload.h"
#include "error.h"

/*
 * Hook functions
*/

///TODO: Implement support for libraries packaged in the mod.
static PyObject* import_hook(PyObject* self,PyObject* args,PyObject* kwargs){
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

/*
 * Helper functions.
*/

/**
 * Replaces the given builtin with the given value and returns the old value.
**/
PyObject* getset_builtin(const char* s,PyObject* nval){
	static PyObject* builtins_str=PyUnicode_InternFromString("__builtins__");
	static PyObject* globals=PyEval_GetGlobals();

	PyObject* builtins;
	PyObject* object_str=PyUnicode_InternFromString(s);

	PyObject* builtin=NULL;

	std::string error;
	bool iserr=false;

	if(builtins_str==NULL or object_str==NULL){
		error="Unicode initialization failed obtaining builtin.";
		iserr=true;
		goto err;
	}

	if(globals!=NULL){
		Py_IncRef(globals);
		builtins=PyObject_GetItem(globals,builtins_str);
		if(builtins==NULL){
			error="__builtins__ not found in globals.";
			iserr=true;
			goto err;
		}
	}
	else{
		//No globals -- use standard builtins,and fake globals
		builtins=PyImport_ImportModuleLevel("builtins",NULL,NULL,NULL,0);
		if(builtins==NULL){
			error="Unable to import builtins.";
			iserr=true;
			goto err;
		}
		globals=Py_BuildValue("{OO}",builtins_str,builtins);
		if(globals==NULL){
			error="Unable to create globals dictionary from __builtins__.";
			iserr=true;
			goto err;
		}
	}

	if(PyDict_Check(builtins)){
		builtin=PyObject_GetItem(builtins,object_str);
		if(builtin==NULL){
			error="__import__ not in builtins.";
			iserr=true;
			goto err;
		}
		PyObject_SetItem(builtins,object_str,builtin);
	}
	else{
		builtin=PyObject_GetAttr(builtins,object_str);
		if(builtin==NULL){
			error="__import__ not a member of builtins.";
			iserr=true;
			goto err;
		}
		PyObject_SetAttr(builtins,object_str,builtin);
	}

	err:
		Py_XDECREF(builtins);
		Py_XDECREF(object_str);
		Py_XDECREF(builtin);

		if(iserr){
			throw HookError(error);
		}

		return builtin;
}

/**
 * Removes the given builtin.
**/
void remove_builtin(const char* s){
	static PyObject* builtins_str=PyUnicode_InternFromString("__builtins__");
	static PyObject* globals=PyEval_GetGlobals();

	PyObject* builtins;
	PyObject* object_str=PyUnicode_InternFromString(s);

	std::string error;
	bool iserr=false;

	if(builtins_str==NULL or object_str==NULL){
		error="Unicode initialization failed obtaining builtin.";
		iserr=true;
		goto err;
	}

	if(globals!=NULL){
		Py_IncRef(globals);
		builtins=PyObject_GetItem(globals,builtins_str);
		if(builtins==NULL){
			error="__builtins__ not found in globals.";
			iserr=true;
			goto err;
		}
	}
	else{
		//No globals -- use standard builtins,and fake globals
		builtins=PyImport_ImportModuleLevel("builtins",NULL,NULL,NULL,0);
		if(builtins==NULL){
			error="Unable to import builtins.";
			iserr=true;
			goto err;
		}
		globals=Py_BuildValue("{OO}",builtins_str,builtins);
		if(globals==NULL){
			error="Unable to create globals dictionary from __builtins__.";
			iserr=true;
			goto err;
		}
	}

	if(PyDict_Check(builtins)){
		if(PyObject_DelItem(builtins,object_str)==-1){
			error="__import__ not in builtins.";
			iserr=true;
			goto err;
		}
	}
	else{
		if(PyObject_DelAttr(builtins,object_str)){
			error="__import__ not a member of builtins.";
			iserr=true;
			goto err;
		}
	}

	err:
		Py_XDECREF(builtins);
		Py_XDECREF(object_str);

		if(iserr){
			throw HookError(error);
		}
}

/*
 * Static initializer methods.
*/

void __overload_static_init::init_import(){
	PyMethodDef hook_descr={
		"__import__",
		(PyCFunction)import_hook,
		METH_KEYWORDS,
		NULL
	};

	PyObject* hook=PyCFunction_New(&hook_descr,NULL);

	original_import=getset_builtin("__import__",hook);
}

void __overload_static_init::init_removals(){
	for(unsigned i=0;blacklisted_builtins[i]!=NULL;++i){
		remove_builtin(blacklisted_builtins[i]);
	}
}

__overload_static_init::__overload_static_init(){
	init_import();
	init_removals();
}
