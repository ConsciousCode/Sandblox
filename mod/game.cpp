#include <Python.h>

#include "game.h"
#include "modwrapper.h"

PyObject* perform_input_hook(PyObject* self,PyObject* args){
	Mod* mod;
	char* type;
	char* tohook;
	PyObject* callback;

	if(PyArg_ParseTuple("OssO",&mod,&type,&tohook,&callback)){
		if(PyCallable_Check(callback)){
			std::string stype(type);
			std::string shook(tohook);
			upper(stype);
			upper(shook);

			//Empty type is a hook-all
			if(stype.length()==0){
				game.input.hooks.push_back(InputHook(mod,ALL,0,callback));
			}
			else if(stype=="KEY"){
				if(shook.length()==0){
					game.input.hooks.push_back(InputHook(mod,ALLKEY,0,callback));
				}
				else{
					int res=find_value(key_names,shook);
					if(res==-1){
						PyErr_SetString(&PyExc_ValueError,"Invalid input hook identifier.");
						return NULL;
					}
					game.input.hooks.push_back(InputHook(mod,KEY,res,callback));
				}
			}
			//Probably a single character key
			else if(stype=="MOUSE"){
				if(shook.length()==0){
					game.input.hooks.push_back(InputHook(mod,ALLMOUSE,0,callback));
				}
				else{
					int res=find_value(mouse_names,shook);
					if(res==-1){
						PyErr_SetString(&PyExc_ValueError,"Invalid input hook identifier.");
						return NULL;
					}
					game.input.hooks.push_back(InputHook(mod,MOUSE,res,callback));
				}
			}
			else{
				PyErr_SetString(&PyExc_ValueError,"Invalid input hook type identifier.");
				return NULL;
			}

			Py_IncRef(Py_None);
			return Py_None;
		}
		PyErr_SetString(&PyExc_ValueError,"Need to pass a valid callable.");
		return NULL;
	}
	PyErr_SetString(&PyExc_ValueError,"Invalid parameters.");
	return NULL;
}

PyObject* remove_input_hook(PyObject* self,PyObject* args){
	Mod* mod;
	char* type;
	char* tohook;

	if(PyArg_ParseTuple("Oss",&mod,&type,&tohook)){
		std::string stype(type);
		std::string shook(tohook);
		upper(stype);
		upper(shook);

		std::vector<InputHook>& hooks=game.input.hooks;

		//Empty type is a hook-all
		if(stype.length()==0){
			for(unsigned i=0;i<hooks.length();++i){
				if(hooks[i].type==ALL and hooks[i].owner->can_metamod(mod)){
					hooks.erase(hooks.begin()+i);
					--i;
				}
			}
		}
		else if(stype=="KEY"){
			if(shook.length()==0){
				for(unsigned i=0;i<hooks.length();++i){
					if(hooks[i].type==ALLKEY and hooks[i].owner->can_metamod(mod)){
						hooks.erase(hooks.begin()+i);
						--i;
					}
				}
			}
			else{
				int res=find_value(key_names,shook);
				if(res==-1){
					PyErr_SetString(&PyExc_ValueError,"Invalid input hook identifier.");
					return NULL;
				}
				for(unsigned i=0;i<hooks.length();++i){
					if(hooks[i].type==KEY and hooks[i].value==res and hooks[i].owner->can_metamod(mod)){
						hooks.erase(hooks.begin()+i);
						--i;
					}
				}
			}
		}
		//Probably a single character key
		else if(stype=="MOUSE"){
			if(shook.length()==0){
				for(unsigned i=0;i<hooks.length();++i){
					if(hooks[i].type==ALLMOUSE and hooks[i].owner->can_metamod(mod)){
						hooks.erase(hooks.begin()+i);
						--i;
					}
				}
			}
			else{
				int res=find_value(mouse_names,shook);
				if(res==-1){
					PyErr_SetString(&PyExc_ValueError,"Invalid input hook identifier.");
					return NULL;
				}
				for(unsigned i=0;i<hooks.length();++i){
					if(hooks[i].type==MOUSE and hooks[i].value==res and hooks[i].owner->can_metamod(mod)){
						hooks.erase(hooks.begin()+i);
						--i;
					}
				}
			}
		}
		else{
			PyErr_SetString(&PyExc_ValueError,"Invalid input hook type identifier.");
			return NULL;
		}

		Py_IncRef(Py_None);
		return Py_None;
	}
	PyErr_SetString(&PyExc_ValueError,"Invalid parameters.");
	return NULL;
}

void init_gamemod(Mod* m){
	PyObject* wrapped_input=ModBinding_New(m,perform_input_hook);
	PyObject* wrapped_remove=ModBinding_New(m,remove_input_hook);

	PyObject* gmod=PyModule_Create2(&game_mod,PYTHON_API_VERSION);
	if(m==NULL){
		return NULL;
	}

	PyModule_AddObject(gmod,"hook",wrapped_input);
	PyModule_AddObject(gmod,"removehook",wrapped_remove);

	return m;
}
