/*
This file is part of Sandblox.

	Sandblox is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Sandblox is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Sandblox.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Python.h>

#include "version.h"

PyMemberDef Version_members[]={
	{(char*)"major",T_UINT,offsetof(Version,major),0,(char*)"Major version"},
	{(char*)"minor",T_UINT,offsetof(Version,minor),0,(char*)"Minor version"},
	{(char*)"build",T_UINT,offsetof(Version,build),0,(char*)"Build version"},
	{(char*)"revision",T_UINT,offsetof(Version,build),0,(char*)"Revision version"}
};

PyTypeObject VersionType={
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

int version_init(Version* self,PyObject* args,PyObject*){
	char* s;
	if(PyArg_ParseTuple((PyObject*)self,"iiii",&self->major,&self->minor,&self->build,&self->revision)){
		return 0;
	}
	else if(PyArg_ParseTuple((PyObject*)self,"s",&s)){
		unsigned a,b,c,d;
		a=b=c=d=0;
		unsigned count=sscanf(s,"%u.%u.%u.%u",&a,&b,&c,&d);
		self->major=a;
		self->minor=b;
		self->build=c;
		self->revision=d;
		if(count==0){
			PyErr_SetString(PyExc_ValueError,"Version string must have a valid version format.");
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

PyObject* version_repr(Version* self){
	return PyBytes_FromFormat("version(%i,%i,%i,%i)",self->major,self->minor,self->build,self->revision);
}

PyObject* version_str(Version* self){
	return PyBytes_FromFormat("%i.%i.%i.%i",self->major,self->minor,self->build,self->revision);
}

PyObject* Version_New(const char* s){
	PyObject* self=(PyObject*)PyObject_New(Version,&VersionType);
	if(self==0){
		return NULL;
	}
	version_init((Version*)self,Py_BuildValue("(s)",s),NULL);
	return self;
}

PyObject* Version_New(unsigned v){
	PyObject* _self=(PyObject*)PyObject_New(Version,&VersionType);
	if(!_self){
		return NULL;
	}

	Version* self=(Version*)_self;

	self->major=(v&0xff000000)>>24;
	self->minor=(v&0x00ff0000)>>16;
	self->build=(v&0x0000ff00)>>8;
	self->revision=(v&0x000000ff);

	return _self;
}
