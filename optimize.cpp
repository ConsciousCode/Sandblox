#include <Python.h>

#include <string>

#include "optimize.h"

static PyCodeObject* storeload_pattern(std::string& s,PyCodeObject* code){
	unsigned x=s.length();

	for(unsigned i=0;i<x;++i){
		if(s[i]==STORE_FAST and s[i+2]==LOAD_FAST){
			if(s[i+1]==s[i+3]){
				s[i]=DUP_TOP;
				s[i+2]=NOP;
			}
		}
		i+=
	}
}

PyObject* peephole(PyObject* code){
}
