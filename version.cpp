#include <Python.h>

#include "version.h"

int version_init(Version* self,PyObject* args,PyObject* kwargs){
	char* s;
	if(PyArg_ParseTuple(self,"iiii",&self->major,&self->minor,&self->build,&self->revision)){
		return 0;
	}
	else if(PyArg_ParseTuple(self,"s",&s)){
		self->major=self->minor=self->build=self->revision=0;
		unsigned count=sscanf(s,"%i.%i.%i.%i",&self->major,&self->minor,&self->build,&self->revision);
		if(count==0){
			PyErr_SetString(&PyExc_ValueError,"Version string must have a valid version format.");
			return -1;
		}
		return 0;
	}
	return -1;
}

unsigned s2version(const std::string& s){
	unsigned major=0,minor=0,build=0,revision=0;
	if(sscanf(s.c_str(),"%i.%i.%i.%i",&major,&minor,&build,&revision)){
		major&=0xff;
		minor&=0xff;
		build&=0xff;
		revision&=0xff;

		return major<<24 | minor<<16 | build<<8 | revision;
	}
	return 0;
}

PyObject* version_repr(PyObject* self){
	return PyBytes_FromFormat("version(%i,%i,%i,%i)",self->major,self->minor,self->build,self->revision);
}

PyObject* version_str(Version* self){
	return PyBytes_FromFormat("%i.%i.%i.%i",self->major,self->minor,self->build,self->revision);
}

PyObject* Version_New(const char* s){
	PyObject* self=PyObject_New(&VersionType,NULL);
	if(self==0){
		return NULL;
	}
	version_init(self,Py_BuildValue("(s)",s));
	return self;
}

PyObject* Version_New(unsigned v){
	PyObject* self=PyObject_New(&VersionType,NULL);
	if(self==0){
		return NULL;
	}

	Version* v=(Version*)self;

	v->major=(v&0xff000000)>>24;
	v->minor=(v&0x00ff0000)>>16;
	v->build=(v&0x0000ff00)>>8;
	v->revision=(v&0x000000ff);

	return self;
}
