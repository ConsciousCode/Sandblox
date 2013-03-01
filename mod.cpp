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
	FILE* file=fopen(out.c_str(),"w");
	fwrite("MOD",sizeof(char),3,file);

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
	name=root.FirstChildElement("name").ValueTStr();

	//The mod's version
	elem=root.FirstChildElement("version");
	if(!elem){
		//Todo
		return;
	}

	//List of authors of the mods
	elem=root.FirstChildElement("authors");
	if(!elem){
		//Todo
		return;
	}

	std::string s=elem->ValueTStr();
	std::string author;
	//Parse the authors
	for(unsigned i=0;i<s.length();++i){
		if(s_authors[i]==','){
			authors.push_back(author);
			author="";
			//Skip whitespace
			for(;i<s.length() and s[i]!=' ';++i){}
		}
		else{
			author+=s[i];
		}
	}

	v=Version_New(elem->Value());

	//The permissions allowed by the mod
	elem=root.FirstChildElement("permissions");
	if(!elem){
		//Todo
		return;
	}

	permissions.readmap=bool(elem->FirstChildElement("readmap"));
	permissions.readmap=bool(elem->FirstChildElement("writemap"));
	permissions.readmap=bool(elem->FirstChildElement("player"));
	permissions.readmap=bool(elem->FirstChildElement("input"));
	permissions.readmap=bool(elem->FirstChildElement("overlay"));
	permissions.readmap=bool(elem->FirstChildElement("render"));

	//The mod's type
	elem=root.FirstChildElement("type");
	if(!elem){
		//Todo
		return;
	}

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

Mod::Mod(const std::string& fname){
	TiXmlDocument metadata(fname+"metadata.xml");
	if(!metadata.LoadFile()){
		///TODO: Implement code to handle invalid mods
		return;
	}
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
	name=root.FirstChildElement("name").ValueTStr();

	//The mod's version
	elem=root.FirstChildElement("version");
	if(!elem){
		//Todo
		return;
	}

	//List of authors of the mods
	elem=root.FirstChildElement("authors");
	if(!elem){
		//Todo
		return;
	}

	std::string s=elem->ValueTStr();
	std::string author;
	//Parse the authors
	for(unsigned i=0;i<s.length();++i){
		if(s_authors[i]==','){
			authors.push_back(author);
			author="";
			//Skip whitespace
			for(;i<s.length() and s[i]!=' ';++i){}
		}
		else{
			author+=s[i];
		}
	}

	v=Version_New(elem->Value());

	//The permissions allowed by the mod
	elem=root.FirstChildElement("permissions");
	if(!elem){
		//Todo
		return;
	}

	permissions.readmap=bool(elem->FirstChildElement("readmap"));
	permissions.readmap=bool(elem->FirstChildElement("writemap"));
	permissions.readmap=bool(elem->FirstChildElement("player"));
	permissions.readmap=bool(elem->FirstChildElement("input"));
	permissions.readmap=bool(elem->FirstChildElement("overlay"));
	permissions.readmap=bool(elem->FirstChildElement("render"));

	//The mod's type
	elem=root.FirstChildElement("type");
	if(!elem){
		//Todo
		return;
	}

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

	//Prepare the environment
	PyObject* builtins=PyDict_Copy(PyModule_GetDict(safe_builtins));
	PyDict_SetItemString(builtins,"__import__",contextualize_import(this));

	environment=PyDict_New();
	PyDict_SetItemString(environment,"__builtins__",builtins);
	PyDict_SetItemString(environment,"__name__",PyBytes_FromString(name.c_str()));
}
