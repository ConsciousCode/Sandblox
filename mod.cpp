#include <Python.h>
#include <structmember.h>

#include "mod.h"

void init_mods(){
	PyObject* m=PyModule_Create2(&Sandblox_mod,PYTHON_API_VERSION);
	if(m==NULL){
		return NULL;
	}
	return m;
}

bool Mod::can_metamod(Mod* m){
	for(unsigned i=0;metamods[i]!=NULL;++i){
		if(metamods[i]==m){
			return true;
		}
	}
	return false;
}
