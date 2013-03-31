#ifndef SANDBLOX_XMLWRAPPER_H
#define SANDBLOX_XMLWRAPPER_H

#include <Python.h>

#include <string>

PyObject* xml_etree_ElementTree;

void init_xml();

struct Element{
	protected:
		PyObject* element;
		PyObject* children;

		PyObject* func_get;
	public:
		Element(PyObject* e);

		~Element(){
			Py_DecRef(e);
		}

		bool valid();

		std::string attr(const std::string& s);

		Element child(unsigned i);
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

		Element root();
};

#endif
