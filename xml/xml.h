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

#ifndef SANDBLOX_XMLWRAPPER_H
#define SANDBLOX_XMLWRAPPER_H

#include <Python.h>

#include <string>

PyObject* xml_etree_ElementTree;

void init_xml();

struct Attribute:public std::string{
	Attribute(const char* s):std::string(s){}
	Attribute(const std::string& s):std::string(s){}

	int asint();
	double asfloat();
};

struct Element{
	protected:
		PyObject* element;
		PyObject* children;

		PyObject* func_get;
		PyObject* func_find;
	public:
		std::string text;

		Element(PyObject* e);

		~Element(){
			Py_DecRef(element);
		}

		operator bool();

		Attribute attr(const std::string& s);

		Element child(unsigned i);
		Element child(const std::string& s);
		Element child(const std::string& s,unsigned i);
		Element operator[](unsigned i);

		Element& operator=(Element& other);
};

struct Document{
	protected:
		PyObject* elemtree;
		PyObject* _root;
	public:
		Document(const std::string& fname);
		~Document();

		operator bool();

		Element root();
};

#endif
