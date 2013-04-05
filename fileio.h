/**
 * Your one-stop shop for all custom file structures used by Sandblox
**/
#ifndef SANDBLOX_FILEIO_H
#define SANDBLOX_FILEIO_H

#include <Python.h>

#include <string>
#include <vector>
#include <map>

#include <cstdio>

#include "endian.h"

struct ChunkHeader{
	char signature[4];
	ulil_t size;
};

void read_string(std::string* sp,FILE* f){
	ulil16_t safe_length;
	fread(&safe_length,sizeof(ulil16_t),1,f);
	unsigned short length=safe_length;
	char* buffer=new char[length];
	fread(buffer,sizeof(char),length,f);
	*sp=buffer;
	delete[] buffer;
}

unsigned short read_short(FILE* f){
	ulil16_t in;
	fread(&in,sizeof(in),1,f);
	return in;
}

unsigned read_int(FILE* f){
	ulil_t in;
	fread(&in,sizeof(in),1,f);
	return in;
}

struct ModChunk{
	struct AuthorContrib{
		std::string author;
		std::string contrib;
	};

	std::string name;
	std::string description;
	ulil_t version;
	std::vector<AuthorContrib> authors;
};

struct ResourceChunk{
	struct File{
		File* parent;

		std::string name;
		ulil16_t size;
		size_t offset;
		std::vector<File> files;
	}root;
};

void read_resource_chunk(ResourceChunk* rc,FILE* f){
	rc->root.parent=NULL;

	File* cur=&rc.root;
	struct iter{
		unsigned length;
		unsigned offset;
	};
	std::stack<iter> offset;

	read_string(&rc->root.name,f);
	cur->size=read_short(f);
	cur->offset=ftell(f);

	offset.push({cur->size,0});

	while(offset.size()>0 and offset.top().offset>offset.top().length){
		if(offset.top().name[offset.top().name.length()-1]=='/'){
			++offset.top().offset;
			cur->files.push_back(ResourceChunk::File());
			cur->files.back().parent=cur;
			cur=&cur->files.back();

			read_string(&cur->name);
			cur->size=read_short(f);
			cur->offset=ftell(f);

			offset.push({cur->size,0});
		}

		cur=cur->parent;
	}
}

struct CodeChunk{
	std::vector<PyObject*> code;
};

struct ExtraChunk{
	std::map<std::string,PyObject*> entries;
};

#endif // SANDBLOX_FILEIO_H
