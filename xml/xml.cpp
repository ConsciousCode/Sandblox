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

#include "xml.h"

int Attribute::asint(){
	return atoi(c_str());
}

double Attribute::asfloat(){
	return atof(c_str());
}

void init_xml(){
	xml_etree_ElementTree=PyImport_ImportModule("xml.etree.ElementTree");
}

std::string strip_whitespace(const std::string& s){
	unsigned from=0,to=s.length()-1;
	for(;from<s.length() and isspace(s[from]);++from){}
	for(;to>0 and isspace(s[to]);--to){}
	if(to<from){
		return "";
	}
	return s.substr(from,to);
}

Element::Element(PyObject* e):element(e){
	if(e){
		children=PyObject_CallFunction(PyObject_GetAttrString(e,"getchildren"),"O",e);
		text=strip_whitespace(PyBytes_AsString(PyObject_GetAttrString(e,"text")));

		func_get=PyObject_GetAttrString(e,"get");
		func_find=PyObject_GetAttrString(e,"find");
	}
	else{
		Py_IncRef(Py_None);
		Py_IncRef(Py_None);
		Py_IncRef(Py_None);

		children=func_get=func_find=Py_None;
		text="";
	}
}

Element::operator bool(){
	return element==Py_None;
}

Attribute Element::attr(const std::string& s){
	if(func_get==Py_None){
		return "";
	}
	PyObject* res=PyObject_CallFunction(func_get,(char*)"s",s.c_str());
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

Element Element::child(const std::string& s){
	if(func_find==Py_None){
		Py_IncRef(Py_None);
		return Py_None;
	}
	return PyObject_CallFunction(func_find,(char*)"s",(s+"[0]").c_str());
}

Element Element::child(const std::string& s,unsigned i){
	if(func_find==Py_None){
		Py_IncRef(Py_None);
		return Py_None;
	}
	char buffer[18];
	sprintf(buffer,"[%i]",i);
	return PyObject_CallFunction(func_find,(char*)"s",(s+buffer).c_str());
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
	elemtree=PyObject_CallFunction(PyObject_GetAttrString(xml_etree_ElementTree,"parse"),(char*)"Os",fname.c_str());
	_root=PyObject_CallFunction(PyObject_GetAttrString(elemtree,"getroot"),NULL);
}

Document::operator bool(){
	return elemtree==Py_None;
}

Element Document::root(){
	return _root;
}
