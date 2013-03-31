#include <Python.h>

#include <string>

#include "mod.h"

#include "util.h"
#include "overload.h"
#include "systemutils.h"

#include "mod/sandblox.h"

#include "xml/xml.h"

void load_mods(){
	DIR* dp=opendir("mods/");
	dirent* dep;

	while((dep=readdir(dp))){

	}
}

///TODO: Actually write this!
void init_mods(){
	PyObject* marshal_lib=PyImport_ImportModule("marshal");
	Mod::marshal_dumps=PyObject_GetAttrString(marshal_lib,"dumps");
	Mod::marshal_loads=PyObject_GetAttrString(marshal_lib,"loads");

	init_overload();

	//PyObject* m=PyModule_Create(&Sandblox_mod);
}

bool Mod::can_metamod(Mod* m){
	for(unsigned i=0;metamods[i]!="";++i){
		if(metamods[i]==m->name){
			return true;
		}
	}
	return false;
}

void Mod::compile(const std::string& f,const std::string& out){
	std::string folder(f);
	if(folder[folder.length()-1]!='/' and folder[folder.length()-1]!='\\'){
		folder+='/';
	}
	Document metadata((folder+"metadata.xml").c_str());
	if(!metadata.LoadFile()){
		///TODO: Implement code to handle invalid mods
		return;
	}

	ModSection ms;
	ResourceSection rs;
	CodeSection cs;
	BloxSection bs;

	Element root(metadata.root());
	if(!root.valid()){
		//See above TODO
		return;
	}
	TiXmlHandle root(elem);

	/*
	 * Mod metadata
	*/

	//Name of the mod
	ms.name=root.FirstChildElement("name").Text();

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
		permissions.writemap=bool(elem->FirstChildElement("writemap"));
		permissions.player=bool(elem->FirstChildElement("player"));
		permissions.input=bool(elem->FirstChildElement("input"));
		permissions.overlay=bool(elem->FirstChildElement("overlay"));
		permissions.render=bool(elem->FirstChildElement("render"));
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

	/*
	 * Mod resources
	*/

	//load resources
	///TODO: do a recursive search
	DIR* dirp=opendir((folder+"resources/"+folder).c_str());
	struct Folder{
		std::string name;
		DIR* dirp;
	};

	if(dirp){
		std::stack<Folder> folders;
		folders.push({folder,dirp});

		dirent* dp;
		//use this to check if it's a folder
		DIR* check;

		while(folders.size()>0){
			while(dp=readdir(folders.top().dirp)){
				check=opendir((folder+"resources/"+folders.top().name+dp->d_name).c_str());
				if(check){
					folders.push({folders.top().name+dp->d_name,check});
				}
				else{
					rs.resources.push_back(folders.top().name+dp->d_name);
				}
			}
			folders.pop();
		}
		closedir(dirp);
	}

	/*
	 * Mod code/libraries
	*/

	//load libraries
	dirp=opendir(folder+"libs/");
	if(dirp){
		std::stack<Folder> folders;
		folders.push({folder,dirp});

		dirent* dp;
		//use this to check if it's a folder
		DIR* check;

		while(folders.size()>0){
			while(dp=readdir(folders.top().dirp)){
				check=opendir((folder+"libs/"+folders.top().name+dp->d_name).c_str());
				if(check){
					folders.push({folders.top().name+dp->d_name,check});
				}
				else{
					cs.codebits.push_back(folders.top().name+dp->d_name);
				}
			}
			folders.pop();
		}
		closedir(dirp);
	}



	/*
	 * Mod block definitions
	*/

	//load block definitions
	dirp=opendir(folder+"blocks/");
	if(dirp){
		std::stack<Folder> folders;
		folders.push({folder,dirp});

		dirent* dp;
		//use this to check if it's a folder
		DIR* check;

		while(folders.size()>0){
			while(dp=readdir(folders.top().dirp)){
				check=opendir((folder+"blocks/"+folders.top().name+dp->d_name).c_str());
				if(check){
					folders.push({folders.top().name+dp->d_name,check});
				}
				else{
					BloxSection::BlockDef* bd=new BloxSection::BlockDef;
					bd->load(folders.top().name+dp->d_name);
					bs.blockdefs.push_back(bd);
				}
			}
			folders.pop();
		}
		closedir(dirp);
	}

	FILE* f=fopen(out.c_str(),"w");

	ms.write(f);
	rs.write(f);
	cs.write(f);
	bs.write(f);
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
	fwrite("MOD ",sizeof(char),4,f);

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
	fwrite("EMBD",sizeof(char),4,f);

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

bool Mod::load(const std::string& fname){
	struct ModHeader{
		char signature[4];//"MOD "
		endian::ulil_t size;
		endian::ulil_t version;
		endian::ulil8_t type;///Dev Note: Is this necessary?
		endian::ulil8_t permission;
	}modhead;

	FILE* f=fopen(fname.c_str(),"r");

	//read static length data
	fread(&modhead,sizeof(modhead),1,f);

	//check signature
	if(!memcmp("MOD ",modhead.signature,4)){
		return false;
	}
	//unsupported type
	if(type>5){
		return false;
	}

	//build the version and permissions
	v=Version_New(modhead.version);
	permissions=(Permissions)modhead.permission;
	type=modhead.type;

	//read the name
	lil8_t length;
	fread(&length,sizeof(length),1,f);
	char* n=new char[length];
	fread(n,sizeof(char),length,f);

	name=n;
	delete[] n;

	//read the authors
	fread(&length,sizeof(length),1,f);

	for(unsigned i=0;i<length;++i){
		lil8_t alen;
		fread(&alen,sizeof(alen),1,f);

		n=new char[alen];
		fread(n,sizeof(char),alen,f);
		authors.push_back(n);
		delete[] n;
	}

	//read the mods that can be metamodded
	fread(&length,sizeof(length),1,f);

	for(unsigned i=0;i<length;++i){
		lil8_t alen;
		fread(&alen,sizeof(alen),1,f);

		n=new char[alen];
		fread(n,sizeof(char),alen,f);
		metamods.push_back(n);
		delete[] n;
	}

	/*
	 * EMBeDed resource section
	*/
	struct EmbedHeader{
		char signature[4];
		endian::ulil_t size;
	}embedhead;

	//read embed static length data
	fread(&embedhead,sizeof(embedhead),1,f);

	//verify signature
	if(!memcmp("EMBD",embedhead.signature,4)){
		return false;
	}

	struct Directory{
		unsigned superpos;
		std::string dir;
		unsigned contained;

		Directory(const Directory& parent,unsigned pos,FILE* f):superpos(pos){
			endian::ulil8_t dlen;
			fread(&dlen,sizeof(dlen),1,f);
			char* buf=new char[dlen];
			fread(buf,sizeof(char),dlen,f);
			dir=parent.dir+'/'+buf;
			delete[] buf;
			fread(&contained,sizeof(contained),1,f);
		}
	};

	std::stack<Directory> fsystem;
	fsystem.push(Directory(0,f));
	unsigned pos=0;

	while(fsystem.size()>0){
		if(fsystem.top().name[fsystem.top().name.length()-1]=='/'){
			//directory
			if(fsystem.top().superpos<pos){
				fsystem.push(Directory(fsystem.top(),pos,f));
				pos=0;
			}
			else{
				pos=fsystem.top().superpos+1;
				fsystem.pop();
			}
		}
		else{
			//file
			resources.push_back(Resource(fsystem.top().name,fsystem.top().contained,f));
			pos=fsystem.top().superpos+1;
			fsystem.pop();
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

void BloxSection::BlockDef::load(const std::string& fname){
	TiXmlDocument block(fname);

	if(!block.LoadFile()){
		return false;
	}

	TiXmlHandle handle(&block);
	TiXmlElement* elem;
	TiXmlHandle root;

	elem=handle.FirstChildElement("block");
	if(!elem){
		return false;
	}
	root=TiXmlHandle(elem);

	if(elem=root.FirstChildElement("name")){
		name=elem.Text();
	}

	if(elem=root.FirstChildElement("description")){
		description=elem.Text();
	}

	if(elem=root.FirstChildElement("light")){
		light=atof(elem.Text().c_str());
	}
	else{
		light=0.0f;
	}

	if(elem=root.FirstChildElement("gravity")){
		gravity=atof(elem.Text().c_str());
	}
	else{
		gravity=0.0f;
	}

	if(elem=root.FirstChildElement("shape")){
		light=elem.Text().c_str();
	}

	if(elem=root.FirstChildElement("skin")){
		light=elem.Text().c_str();
	}
}

void BloxSection::write(FILE* f){
	unsigned tmp=blockdefs.size();

	fwrite(&tmp,sizeof(tmp),1,f);

	for(unsigned i=0;i<tmp;++i){
		blockdefs[i]->write(f);
	}
}
