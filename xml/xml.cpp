#include "xml.h"

Element::Element(PyObject* e):element(e){
	children=PyObject_CallFunction(PyObject_GetAttrString(e,"getchildren"),NULL);
	func_get=PyObject_GetAttrString(e,"get");

	if(!children){
		Py_IncRef(Py_None);
		children=Py_None;
	}
	if(!func_get){
		Py_IncRef(Py_None);
		func_get=Py_None;
	}
}

bool Element::valid(){
	return element==Py_None;
}

std::string Element::attr(const std::string& s){
	if(func_get==Py_None){
		return "";
	}
	PyObject* res=PyObject_CallFunction(func_get,"s",s.c_str());
	if(res==Py_None){
		return "";
	}
	return PyBytes_AsString(res);
}

Element Element::child(unsigned i){
	if(element==Py_None){
		Py_IncRef(Py_None);
		return Py_None;
	}
	PyObject* pyi=PyLong_FromLong(i);
	PyObject* res=PyObject_GetItem(element,pyi);
	if(!res){
		Py_IncRef(Py_None);
		return Py_None;
	}
	Py_DecRef(pyi);
	return res;
}

Element Element::operator[](unsigned i){
	return child(i);
}

Element& Element::operator=(Element& other){
	Py_DecRef(element);
	element=other.element;
	Py_IncRef(element);

	Py_DecRef(children);
	children=other.children;
	Py_IncRef(children);

	Py_DecRef(func_get);
	func_get=other.func_get;
	Py_IncRef(func_get);

	return *this;
}

Document::Document(const std::string& fname){
	elemtree=PyObject_CallFunction(PyObject_GetAttrString(xml_etree_ElementTree,"ElementTree"),"Os",Py_None,fname.c_str());
	_root=PyObject_CallFunction(PyObject_GetAttrString(elemtree,"getroot"),NULL);
}

Element Document::root(){
	return _root;
}
