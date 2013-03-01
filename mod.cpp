#include <Python.h>

#include <string>

#include "mod.h"
#include "util.h"
#include "overload.h"
#include "systemutils.h"

#include "../xml/tinyxml.h"

void load_mods(){
	DIR* dp=opendir("mods/");
	dirent* dep;

	while(dep=readdir(dp)){

	}
}

void init_mods(){
	PyObject* marshal_lib=PyImport_Import("marshal");
	marshal_dumps=PyObject_GetAttrString(marshal_lib,"dumps");
	marshal_loads=PyObject_GetAttrString(marshal_lib,"loads");

	init_overload();

	PyObject* m=PyModule_Create(&Sandblox_mod);
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

void Mod::compile(const std::string& folder,const std::string& out){
	if(folder[folder.length()-1]!='/' and folder[folder.length()-1]!='\\'){
		folder+='/';
	}
	TiXmlDocument metadata(folder+"metadata.xml");
	if(!metadata.LoadFile()){
		///TODO: Implement code to handle invalid mods
		return;
	}

	ModSection ms;
	ResourceSection rs;
	BloxSection bs;

	TiXmlHandle handle(&metadata);
	TiXmlElement* elem;
	TiXmlHandle root;

	elem=handle.FirstChildElement();
	if(!elem){
		//See above TODO
		return;
	}
	root=TiXmlHandle(elem);

	//Name of the mod
	ms.name=root.FirstChildElement("name").ValueTStr();

	//Description of the mod
	ms.description=root.FirstChildElement("description").ValueTStr();

	//The mod's version
	elem=root.FirstChildElement("version");
	if(elem){
		ms.version=s2version(elem->ValueTStr());
	}

	//List of authors of the mods
	elem=root.FirstChildElement("authors");
	if(elem){
		std::string s=elem->ValueTStr();
		std::string author;
		//Parse the authors
		for(unsigned i=0;i<s.length();++i){
			if(ms.authors[i]==','){
				ms.authors.push_back(author);
				author="";
				//Skip whitespace
				for(;i<s.length() and s[i]!=' ';++i){}
			}
			else{
				author+=s[i];
			}
		}
	}

	//The permissions allowed by the mod
	elem=root.FirstChildElement("permissions");
	if(elem){
		permissions.readmap=bool(elem->FirstChildElement("readmap"));
		permissions.readmap=bool(elem->FirstChildElement("writemap"));
		permissions.readmap=bool(elem->FirstChildElement("player"));
		permissions.readmap=bool(elem->FirstChildElement("input"));
		permissions.readmap=bool(elem->FirstChildElement("overlay"));
		permissions.readmap=bool(elem->FirstChildElement("render"));
	}

	//The mod's type
	elem=root.FirstChildElement("type");
	if(elem){
		s=elem->GetText();
		upper(s);
		if(s=="PACKAGE"){
			type=Mod::PACKAGE;
		}
		else if(s=="WORLD"){
			type=Mod::WORLD;
		}
		else if(s=="GENERATOR"){
			type=Mod::GENERATOR;
		}
		else if(s=="META"){
			type=Mod::META;
		}
		else if(s=="VIRAL"){
			type=Mod::VIRAL;
		}
		else if(s=="CONTENT"){
			type=Mod::CONTENT;
		}
		else if(s=="RESOURCE"){
			type=Mod::RESOURCE;
		}
		else{
			//Invalid type, handle
			return;
		}
	}

	elem=root.FirstChildElement("meta");
	if(elem){
		std::string s=elem->ValueTStr();
		std::string mod;
		//Parse the authors
		for(unsigned i=0;i<s.length();++i){
			if(ms.metamods[i]==','){
				ms.metamods.push_back(mod);
				mod="";
				//Skip whitespace
				for(;i<s.length() and s[i]!=' ';++i){}
			}
			else{
				mod+=s[i];
			}
		}
	}

	//load resources
	///TODO: do a recursive search
	DIR* dirp=opendir((folder+"resources/").c_str());
	if(dirp){
		dirent* dp;
		//use this to check if it's a folder
		DIR* check;

		while(dp=readdir(dirp)){
			check=opendir((folder+"resources/"+dp->d_name).c_str());
			if(!check){
				closedir(check);
				rs.resources.push_back(dp->d_name);
			}
		}
	}

	//load code
	///TODO: do a recursive search
	DIR* dirp=opendir((folder+"code/"));
	if(dirp){
		dirent* dp;
		PyObject* code;
		FILE* f;

		while(dp=readdir(dirp)){
			f=fopen(dp->d_name,"r");
			code=PyNode_Compile(PyParser_SimpleParseFile())
			fclose(f);
			if(code){
				rs.resources.push_back(code);
			}
			else{
				//probably syntax error
				///TODO: Alert the user of this error
				//for now, ignore
				PyErr_Clear();
			}
		}
	}
}

Mod::Mod(const std::string& fname){

	//Prepare the environment
	PyObject* builtins=PyDict_Copy(PyModule_GetDict(safe_builtins));
	PyDict_SetItemString(builtins,"__import__",contextualize_import(this));

	environment=PyDict_New();
	PyDict_SetItemString(environment,"__builtins__",builtins);
	PyDict_SetItemString(environment,"__name__",PyBytes_FromString(name.c_str()));
}

/*
 * Mod file section write methods
*/

void ModSection::write(FILE* f){
	//signature
	fwrite("MOD",sizeof(char),3,f);

	//numeric members
	fwrite(&chunklength,sizeof(chunklength),1,f);
	fwrite(&version,sizeof(version),1,f);
	fwrite(&type,sizeof(type),1,f);
	fwrite(&permission,sizeof(permission),1,f);

	//name
	unsigned tmp=name.length();

	fwrite(&tmp,sizeof(tmp),1,f);
	fwrite(name.c_str(),sizeof(char),tmp,f);

	//description
	tmp=description.length();

	fwrite(&tmp,sizeof(tmp),1,f);
	fwrite(description.c_str(),sizeof(char),tmp,f);

	//authors
	tmp=authors.size();
	unsigned x;

	fwrite(&tmp,sizeof(tmp),1,f);

	for(unsigned i=0;i<tmp;++i){
		x=authors[i].length();
		fwrite(&x,sizeof(x),1,f);
		fwrite(authors[i].c_str(),sizeof(char),x,f);
	}

	//metamods
	tmp=metamods.size();

	fwrite(&tmp,sizeof(tmp),1,f);

	for(unsigned i=0;i<tmp;++i){
		x=metamods[i].length();
		fwrite(&x,sizeof(x),1,f);
		fwrite(metamods[i].c_str(),sizeof(char),x,f);
	}
}

void ResourceSection::write(FILE* f){
	//signature
	fwrite("EMBED",sizeof(char),5,f);

	//numeric members
	fwrite(&chunklength,sizeof(chunklength),1,f);

	//resources
	unsigned tmp=resources.size();
	unsigned x;
	std::string s;
	FILE* rc;

	fwrite(&tmp,sizeof(tmp),1,f);

	for(unsigned i=0;i<tmp;++i){
		s=resources[i];
		x=s.length();
		rc=fopen(s.c_str(),"r");
		if(!rc){
			continue;
		}
		//write filename
		fwrite(&x,sizeof(x),1,f);
		fwrite(s.c_str(),sizeof(char),x,f);

		//copy file
		while(!feof(rc)){
			fputc(fgetc(f),rc);
		}
	}
}

void CodeSection::write(FILE* f){
	//signature
	fwrite("EMBED",sizeof(char),5,f);

	//numeric members
	fwrite(&chunklength,sizeof(chunklength),1,f);

	//code bits
	unsigned tmp=codebits.size();
	unsigned x;
	PyObject* out;

	fwrite(&tmp,sizeof(tmp),1,f);

	for(unsigned i=0;i<tmp;++i){
		out=PyObject_Call(codebits[i]);
		if(out){
			x=PyBytes_Size(out);
			fwrite(&x,sizeof(x),1,f);
			fwrite(PyBytes_AsString(out),sizeof(char),x,f);
		}
		else{
			//Just ignore the invalid code object
			PyErr_Clear();
		}
	}
}

void BloxSection::BlockDef::write(FILE* f){
	//name
	unsigned tmp=name.length();

	fwrite(&tmp,sizeof(tmp),1,f);
	fwrite(name.c_str(),sizeof(char),tmp,f);

	//description
	tmp=description.length();

	fwrite(&tmp,sizeof(tmp),1,f);
	fwrite(description.c_str(),sizeof(char),tmp,f);

	//numeric members
	fwrite(&light,sizeof(light),1,f);
	fwrite(&gravity,sizeof(gravity),1,f);

	//code name
	tmp=code.length();
	fwrite(&tmp,sizeof(tmp),1,f);
	fwrite(code.c_str(),sizeof(char),tmp,f);

	//mesh
	tmp=mesh.length();

	fwrite(&tmp,sizeof(tmp),1,f);
	fwrite(mesh.c_str(),sizeof(char),tmp,f);

	//skin
	tmp=skin.length();

	fwrite(&tmp,sizeof(tmp),1,f);
	fwrite(skin.c_str(),sizeof(char),tmp,f);
}

void BloxSection::write(FILE* f){
	unsigned tmp=blockdefs.size();

	fwrite(&tmp,sizeof(tmp),1,f);

	for(unsigned i=0;i<tmp;++i){
		blockdefs[i]->write(f);
	}
}
