#ifndef SANDBLOX_MAP_H
#define SANDBLOX_MAP_H

#include <Python.h>

#include <map>
#include <string>
#include <cctype>

struct DNA{
	PyOBJECT_HEAD
	std::map<std::string,PyObject*> data;

	PyObject* operator[](const std::string& s){
		return data[s];
	}

	PyObject* process_codon(const std::string& type,const std::string& value){
		if(type=="int"){
			return PyLong_FromString(value.c_str(),NULL,0);
		}
		else if(type=="float"){
			return PyFloat_FromString(value.c_str());
		}
		else if(type=="color"){
			return Color_FromString(value.c_str());
		}
		else if(type=="string"){
			return PyBytes_FromString(value.c_str());
		}
	}

	/**
	 * Load DNA from a string using newline-separated terms of the following format.
	 *
	 * <type>:<name>:<value/range/verifier>
	**/
	void load(const std::string& s){
		std::string type,name,value;
		PyObject* res;
		char pos=-1;
		char c;

		for(unsigned i=0;i<s.length();++i){
			c=s[i];
			if(c=='\r' or c=='\n'){
				if(type!="" and name!="" and value!=""){
					res=process_codon(type,value);
					if(res){
						data[name]=res;
					}
				}
				type="";
				name="";
				value="";
				pos=-1;
				for(;i<s.length() and isspace(s[i]);++i){}
				--i;
			}
			else if(isspace(c)){
				for(;i<s.length() and isspace(s[i]) and s[i]!='\r' and s[i]!='\n';++i){}
				++pos;
			}
			else if(pos<0){
				type+=c;
			}
			else if(pos==0){
				name+=c;
			}
			else if(pos>0){
				value+=c;
			}
		}
	}
};

PyObject* DNA_New(){
	DNA* obj=PyObject_New(ColorObject,&Color_Type);
	return (PyObject*)obj;
}

PyObject* DNA_Load(const std::string& s){
	DNA* obj=DNA_New();
	obj->load(s);
	return obj;
}

#endif
