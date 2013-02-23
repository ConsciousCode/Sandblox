#ifndef SANDBLOX_CHUNK_H
#define SANDBLOX_CHUNK_H

#include <Python.h>
#include <structmember.h>

#include <OgreOctree.h>

#include <stack>
#include <cstdio>
#include <math>

#define IDSIZE sizeof(short)
#define MAXDEPTH 5

typedef unsigned short BlockID;

const BlockID NOTLEAF=0xffff;

/**
 * Represents chunking within SANDBLOX using an octree implementation.
**/
struct Chunk{
	PyObject_HEAD
	/**
	 * The chunk's parent chunk (if any)
	**/
	Chunk* parent;

	/**
	 * An array of subchunks.
	**/
	Chunk* subchunks[8];

	/**
	 * The block ID of the chunk
	 *
	 * Note: This is only relevant if it's a leaf node.
	**/
	BlockID id;

	/**
	 * A flagged byte representing which faces are exposed to speed rendering.
	**/
	unsigned char exposed;

	/**
	 * Create a chunk with the given block ID.
	 *
	 * @param i - The block ID.
	**/
	Chunk(BlockID i):id(i){
		subchunks={NULL};
	}

	~Chunk(){
		if(subchunks[0]){
			for(unsigned i=0;i<8;++i){
				delete subchunks[i];
			}
		}
	}

	/**
	 * Divide the chunk into 8 subchunks.
	**/
	void subchunk(){
		if(subchunks[0]){
			return;
		}
		for(unsigned i=0;i<8;++i){
			subchunks[i]=new Chunk(id);
		}
	}

	/**
	 * Load a chunk from a preloaded file.
	 *
	 * @param file - The file handle.
	**/
	void load(FILE* file){
		Chunk* chunk=new Chunk;
		fread(&chunk->id,IDSIZE,1,file);
		if(chunk->id!=NOTLEAF){
			return chunk;
		}
		std::stack<unsigned char> pos;
		pos.push(0);
		Chunk* cur=chunk;
		unsigned short id;

		for(;;){
			fread(&id,IDSIZE,1,file);

			if(id==NOTLEAF){
				cur=cur->subchunks[pos.top()];
				cur->subchunk();
				pos.push(0);
				continue;
			}

			cur->subchunks[pos.top()]->id=id;
			if((++pos.top())>=8){
				pos.pop();
				if(pos.size()==0){
					break;
				}
				++pos.top();
				cur=cur->parent;
			}
		}
	}

	/**
	 * Save the chunk to a file
	 *
	 * @param file - The file handle.
	**/
	void save(FILE* file){
		Chunk* cur=this;
		std::stack<unsigned char> pos;
		pos.push(0);

		for(;;){
			if(cur->subchunks[0]){
				cur=cur->subchunks[pos.top()];
				pos.push(0);
				fwrite(&NOTLEAF,IDSIZE,1,file);
			}
			fwrite(&cur->id,IDSIZE,1,file);

			if((++pos.top())>=8){
				pos.pop();
				if(pos.size()==0){
					break;
				}
				++pos.top();
				cur=cur->parent;
			}
		}
	}

	/**
	 * Get the chunk that occupies the given coordinates.
	 *
	 * @param x - x coordinate
	 * @param y - y coordinate
	 * @param z - z coordinate
	 *
	 * @return The smallest chunk that occupies the coordinates.
	**/
	Chunk* operator()(unsigned x,unsigned y,unsigned z){
		unsigned depth=MAXDEPTH-1;
		unsigned p=1<<depth;

		if(x>=p or y>=p or z>=p){
			return NULL;
		}

		Chunk* cur=this;

		while(cur->subchunks[0]){
			cur=cur->subchunks[(z/p)*4+(y/p)*2+(x/p)];
			x%=p;
			y%=p;
			z%=p;
			p=1<<--depth;
		}
		return cur;
	}

	/**
	 * Set the block at the given coordinates to the given block ID.
	 * This will split larger chunks into smaller chunks if necessary.
	 *
	 * @param x - The x coordinate.
	 * @param y - The y coordinate.
	 * @param z - The z coordinate.
	 * @param i - The block ID.
	**/
	void set(unsigned x,unsigned y,unsigned z,BlockID i){
		unsigned depth=MAXDEPTH-1;
		unsigned p=1<<depth;

		if(x>=p or y>=p or z>=p){
			return NULL;
		}

		Chunk* cur=this;

		while(cur->subchunks[0]){
			cur=cur->subchunks[(z/p)*4+(y/p)*2+(x/p)];
			x%=p;
			y%=p;
			z%=p;
			p=1<<--depth;
		}
		while(depth>0){
			cur->subchunk();
			cur=cur->subchunks[(z/p)*4+(y/p)*2+(x/p)];
			x%=p;
			y%=p;
			z%=p;
			p=1<<--depth;
		}
		cur->id=i;
	}
};

PyObject* Chunk_New()

#endif
