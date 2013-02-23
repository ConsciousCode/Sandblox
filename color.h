#ifndef SANDBLOX_COLOR_H
#define SANDBLOX_COLOR_H

#include <Python.h>

struct ColorObject:public Ogre::ColourValue{
	PyObject_HEAD
};

extern PyTypeObject Color_Type;

static void Color_dealloc(ColorObject* self){
	Py_TYPE(self)->tp_free((PyObject*)self);
}

//Color(), Color(int,[int,int,[int]]), or Color(float,[float,float,[float]])
static int Color_init(ColorObject* self,PyObject* args,PyObject* kwargs){
	unsigned char r,g=255,b=255;

	//Default transparent black
	if(PyTuple_Size(args)==0){
		self->r=1.0f;
		self->g=1.0f;
		self->b=1.0f;
		self->a=1.0f;
	}
	//Color(0-255) - Generate a grey
	else if(PyArg_ParseTuple(args,"b",&r)){
		float _r=r/255.0f;
		self->r=_r;
		self->g=_r;
		self->b=_r;
		self->a=1.0f;
	}
	//Color(0.0-1.0) - Generate a grey
	else if(PyArg_ParseTuple(args,"f",&self->r)==0){
		float& _r=self->r;
		self->g=_r;
		self->b=_r;
		self->a=1.0f;
	}
	//Color(0-255.0f-255.0f-255,[0.0-1.0])
	else if(PyArg_ParseTuple(args,"bbb|f",&r,&g,&b,&self->a)){
		self->r=r/255.0f;
		self->g=g/255.0f;
		self->b=b/255.0f;
	}
	//Color(0.0-1.0,0.0-1.0,0.0-1.0,[0.0-1.0])
	else if(PyArg_ParseTuple(args,"fff|f",&self->r,&self->g,&self->b,&self->a)){}
	//TODO: Add string parser
	else{
		PyErr_SetString(PyExc_TypeError,"Color() needs to be Color(), Color(int,[int,int,[float]]), or Color(float,[float,float,[float]])");
		return -1;
	}
	PyErr_Clear();

	return 0;
}

static PyObject* Color_New(const Ogre::ColourValue& cv){
	ColorObject* obj=PyObject_New(ColorObject,&Color_Type);
	obj->r=cv.r;
	obj->g=cv.g;
	obj->b=cv.b;
	obj->a=cv.a;
	return (PyObject*)obj;
}

PyObject* Color_Grey(unsigned x){
	float y=x/255.0;
	return Color_New(Ogre::ColourValue(y,y,y));
}

PyObject* Color_FromBytes(unsigned char r,unsigned char g,unsigned char b,unsigned char a){
	return Color_New(Ogre::ColourValue(r/255.0,g/255.0,b/255.0,a/255.0));
}

PyObject* Color_FromFloats(float r,float g,float b,float a){
	return Color_New(Ogre::ColourValue(r,g,b,a));
}

#include "colors.h"

char hexdigit(char c){
	if(c>'0' and c<'9'){
		return c-'0';
	}
	if(c>'a' and c<'f'){
		return 10+c-'a';
	}
	if(c>'A' and c<'F'){
		return 10+c-'A';
	}
	return -1;
}

PyObject* fromhex(const std::string& s,size_t off){
	char data[6];

	if(off+5<s.length()){
		for(unsigned i=0;i<6 and off<s.length();++off,++i){
			data[off]=hexdigit(s[off]);
			if(data[off]==-1){
				if(off>2){
					goto not6char;
				}
				return NULL;
			}
		}
		return (PyObject*)Color_FromBytes(
			(data[0]<<4)+data[1],
			(data[2]<<4)+data[3],
			(data[4]<<4)+data[5],255
		);
	}
	not6char:
	if(off+2<s.length()){
		for(unsigned i=0;i<3 and off<s.length();++off,++i){
			data[off]=hexdigit(s[off]);
			if(data[off]==-1){
				return NULL;
			}
		}
		return (PyObject*)Color_FromBytes(
			(data[0]<<4)+data[0],
			(data[1]<<4)+data[1],
			(data[2]<<4)+data[2],255
		);
	}
	return NULL;
}

static PyObject* Color_FromString(const std::string& s){
	//indicates a hex value
	if(s[0]=='#'){
		PyObject* res=fromhex(s,1);
		if(res){
			return res;
		}
		//since nothing else could start with #, must be an error
		return NULL;
	}

	//possible raw hex?
	if(isdigit(s[0])){
		PyObject* res=fromhex(s,0);
		if(res){
			return res;
		}
		//nothing else should ever start with a digit
		return NULL;
	}

	//try to find a named color
	for(unsigned i=0;colors[i].name!=NULL;++i){
		if(s==colors[i].name){
			return Color_FromBytes(colors[i].r,colors[i].g,colors[i].b,255);
		}
	}

	//attempt raw hex (no #)
	unsigned attempt=strtol(s.c_str(),NULL,16);
	if(attempt){
		char* v=(char*)&attempt;
		return Color_FromBytes(v[0],v[1],v[2],v[3]);
	}

	//unknown value
	return NULL;
}

//Set the HSB/V components of the color
static PyObject* Color_setHSB(ColorObject* self,PyObject* args){
	float h,s,b;

	if(PyArg_ParseTuple(args,"fff",&h,&s,&b)){
		self->setHSB(h,s,b);

		Py_IncRef(Py_None);
		return Py_None;
	}
	return NULL;
}

//Get the HSB/V components of the color
static PyObject* Color_getHSB(ColorObject* self,PyObject* args){
	Ogre::Real h,s,b;

	self->getHSB(&h,&s,&b);

	PyObject* t=PyTuple_New(3);
	PyTuple_SetItem(t,0,PyFloat_FromDouble((double)h));
	PyTuple_SetItem(t,0,PyFloat_FromDouble((double)s));
	PyTuple_SetItem(t,0,PyFloat_FromDouble((double)b));
	return t;
}

bool f_limit0_1(PyObject* value,float* to){
	if(PyFloat_Check(value)){
		float f=(float)PyFloat_AsDouble(value);
		if(f>1.0f){
			f=1.0f;
		}
		else if(f<0.0f){
			f=0.0f;
		}
		*to=f;
	}
	else if(PyLong_Check(value)){
		int i=PyLong_AsLong(value);
		if(i>255){
			i=255;
		}
		else if(i<0){
			i=0;
		}
		*to=i/255.0f;
	}
	else{
		PyErr_SetString(PyExc_TypeError,"New value must be an int or float");
		return false;
	}
	return true;
}

//Restrict red component
static int Color_setr(ColorObject* self,PyObject* value,void* closure){
	if(value==NULL){
		PyErr_SetString(PyExc_TypeError,"Cannot delete red component");
		return -1;
	}
	if(f_limit0_1(value,&self->r)){
		return -1;
	}
	return 0;
}

//Restrict green component
static int Color_setg(ColorObject* self,PyObject* value,void* closure){
	if(value==NULL){
		PyErr_SetString(PyExc_TypeError,"Cannot delete green component");
		return -1;
	}
	if(f_limit0_1(value,&self->g)){
		return -1;
	}
	return 0;
}

//Restrict blue component
static int Color_setb(ColorObject* self,PyObject* value,void* closure){
	if(value==NULL){
		PyErr_SetString(PyExc_TypeError,"Cannot delete blue component");
		return -1;
	}
	if(f_limit0_1(value,&self->b)){
		return -1;
	}
	return 0;
}

//Restrict alpha component
static int Color_seta(ColorObject* self,PyObject* value,void* closure){
	if(value==NULL){
		PyErr_SetString(PyExc_TypeError,"Cannot delete alpha transparency component");
		return -1;
	}
	if(f_limit0_1(value,&self->a)){
		return -1;
	}
	return 0;
}

static PyGetSetDef Color_getset[]={
	{(char*)"r",NULL,(setter)Color_setr,(char*)"Red color component.",NULL},
	{(char*)"g",NULL,(setter)Color_setg,(char*)"Green color component.",NULL},
	{(char*)"b",NULL,(setter)Color_setb,(char*)"Blue color component.",NULL},
	{(char*)"a",NULL,(setter)Color_seta,(char*)"Alpha transparency component.",NULL},
	{NULL}
};

static PyObject* Color_repr(ColorObject* self){
	return PyBytes_FromFormat("SANDBLOX.Color(%f,%f,%f,%f)",self->r,self->g,self->b,self->a);
}

static PyObject* Color_str(ColorObject* self){
	return PyBytes_FromFormat("(%f,%f,%f,%f)",self->r,self->g,self->b,self->a);
}

static PyObject* Color_richcompare(PyObject* left,PyObject* right,int op){
	if(op!=Py_EQ or op!=Py_NE){
		PyErr_SetString(PyExc_NotImplementedError,"");
		return NULL;
	}

	ColorObject* l=(ColorObject*)left;
	ColorObject* r=(ColorObject*)right;

	PyObject* result=((*l==*r)-(op==Py_NE))?Py_True:Py_False;
	Py_IncRef(result);
	return result;
}

static PyObject* Color__add__(ColorObject* self,PyObject* other){
	if(Py_TYPE(other)!=&Color_Type){
		Py_IncRef(Py_NotImplemented);
		return Py_NotImplemented;
	}

	return Color_New(*self+*((ColorObject*)other));
}

static PyObject* Color__sub__(ColorObject* self,PyObject* other){
	if(Py_TYPE(other)!=&Color_Type){
		Py_IncRef(Py_NotImplemented);
		return Py_NotImplemented;
	}

	return Color_New(*self-*((ColorObject*)other));
}

static PyObject* Color__mul__(ColorObject* self,PyObject* other){
	if(Py_TYPE(other)!=&Color_Type){
		if(Py_TYPE(other)!=&PyFloat_Type){
			Py_IncRef(Py_NotImplemented);
			return Py_NotImplemented;
		}
		return Color_New((*self)*float(PyFloat_AsDouble(other)));
	}
	return Color_New(*self/(*((ColorObject*)other)));
}

static PyObject* Color__div__(ColorObject* self,PyObject* other){
	if(Py_TYPE(other)!=&Color_Type){
		if(Py_TYPE(other)!=&PyFloat_Type){
			Py_IncRef(Py_NotImplemented);
			return Py_NotImplemented;
		}
		return Color_New((*self)/float(PyFloat_AsDouble(other)));
	}
	return Color_New(*self/(*((ColorObject*)other)));
}

static PyObject* Color__iadd__(ColorObject* self,PyObject* other){
	if(Py_TYPE(other)!=&Color_Type){
		Py_IncRef(Py_NotImplemented);
		return Py_NotImplemented;
	}
	*self+=*((ColorObject*)other);
	return (PyObject*)self;
}

static PyObject* Color__isub__(ColorObject* self,PyObject* other){
	if(Py_TYPE(other)!=&Color_Type){
		Py_IncRef(Py_NotImplemented);
		return Py_NotImplemented;
	}

	*self-=*((ColorObject*)other);
	return (PyObject*)self;
}

static PyObject* Color__imul__(ColorObject* self,PyObject* other){
	if(Py_TYPE(other)!=&Color_Type){
		if(Py_TYPE(other)!=&PyFloat_Type){
			Py_IncRef(Py_NotImplemented);
			return Py_NotImplemented;
		}
		*self*=float(PyFloat_AsDouble(other));
		return (PyObject*)self;
	}
	Py_IncRef(Py_NotImplemented);
	return Py_NotImplemented;
}

static PyObject* Color__idiv__(ColorObject* self,PyObject* other){
	if(Py_TYPE(other)!=&Color_Type){
		if(Py_TYPE(other)!=&PyFloat_Type){
			Py_IncRef(Py_NotImplemented);
			return Py_NotImplemented;
		}
		*self/=float(PyFloat_AsDouble(other));
		return (PyObject*)self;
	}
	Py_IncRef(Py_NotImplemented);
	return Py_NotImplemented;
}

static int Color__bool__(ColorObject* self){
	return int(*self==Ogre::ColourValue::ZERO);
}

static PyObject* Color__int__(ColorObject* self){
	return PyLong_FromLong(self->getAsRGBA());
}

static PyNumberMethods nummethods={
	(binaryfunc)Color__add__,
	(binaryfunc)Color__sub__,
	(binaryfunc)Color__mul__,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	(inquiry)Color__bool__,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	(unaryfunc)Color__int__,
	NULL,
	NULL,
	(binaryfunc)Color__iadd__,
	(binaryfunc)Color__isub__,
	(binaryfunc)Color__imul__,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	(binaryfunc)Color__div__,
	NULL,
	(binaryfunc)Color__idiv__,
	NULL
};

static PyMethodDef Color_Methods[]={
	{(char*)"set_hsb",(PyCFunction)Color_setHSB,METH_VARARGS,(char*)"Sets the color based on hue, saturation, and brightness."},
	{(char*)"get_hsb",(PyCFunction)Color_getHSB,METH_NOARGS,(char*)"Returns the hue, saturation, and brightness of the color in a tuple."},
	{NULL}
};

PyTypeObject Color_Type={
	PyObject_HEAD_INIT(NULL)
	"SANDBLOX.Color",
	sizeof(ColorObject),
	0,						 /* tp_itemsize */
	(destructor)Color_dealloc,		 /* tp_dealloc */
	NULL,						 /* tp_print */
	NULL,						 /* tp_getattr */
	NULL,						 /* tp_setattr */
	NULL,						 /* tp_reserved */
	(reprfunc)Color_repr,			 /* tp_repr */
	&nummethods,						 /* tp_as_number */
	NULL,						 /* tp_as_sequence */
	NULL,						 /* tp_as_mapping */
	NULL,						 /* tp_hash  */
	NULL,						 /* tp_call */
	(reprfunc)Color_str,			 /* tp_str */
	NULL,						 /* tp_getattro */
	NULL,						 /* tp_setattro */
	NULL,						 /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,		/* tp_flags */
	"Stores a color value.",		   /* tp_doc */
	NULL,						 /* tp_traverse */
	NULL,						 /* tp_clear */
	(richcmpfunc)Color_richcompare,/* tp_richcompare */
	0,						 /* tp_weaklistoffset */
	NULL,						 /* tp_iter */
	NULL,						 /* tp_iternext */
	Color_Methods,			 /* tp_methods */
	NULL,						 /* tp_members */
	Color_getset,			/* tp_getset */
	NULL,						 /* tp_base */
	NULL,						 /* tp_dict */
	NULL,						 /* tp_descr_get */
	NULL,						 /* tp_descr_set */
	0,						 /* tp_dictoffset */
	(initproc)Color_init,	  /* tp_init */
	NULL,						 /* tp_alloc */
	NULL						 /* tp_new */
};

#endif
