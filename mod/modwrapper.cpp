#include <Python.h>

#include "modwrapper.h"

PyObject* ModBinding_call(ModBinding* self,PyObject* args,PyObject* kwargs){
	unsigned size=PyTuple_Size(args);
	PyObject* proxied_args=PyTuple_New(size+1);

	PyTuple_SetItem(proxied_args,0,(PyObject*)self->mod);

	for(unsigned i=0;i<size;++i){
		PyTuple_SetItem(proxied_args,i+1,PyTuple_GetItem(args,i));
	}

	return self->callback(self->callback,proxied_args);
}

PyObject* ModBinding_New(Mod* m,PyCFunction f){
	ModBinding* self=(ModBinding*)PyObject_New(&ModBinding_Type,NULL);
	self->mod=m;
	self->callback=f;

	return (PyObject*)self;
}
