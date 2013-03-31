#ifndef SANDBLOX_MOD_H
#define SANDBLOX_MOD_H

#include <Python.h>

#include <string>
#include <vector>

#include "endian.h"
#include "version.h"

/**
 * Initialize Python modules and mod-handling routines.
**/
void init_mods();

/**
 * Load all available mods.
**/
void load_mods();

/**
 * Abstract representation of a resource.
**/
struct Resource{
	std::string name;

	unsigned char* data;
	unsigned length;

	Resource(const std::string& s,unsigned l,FILE* f):name(s),length(l){
		data=new unsigned char[l];
		fread(data,sizeof(char),l,f);
	}
};

/**
 * Structure of mod privileges.
**/
struct Permissions{
	//Access privileges

	/**
	 * Padding bits reserved for later usage.
	**/
	bool res1:1;
	bool res2:1;

	/**
	 * Read/write access to the map.
	**/
	bool readmap:1;
	bool writemap:1;

	/**
	 * Read/write access to the player.
	**/
	bool player:1;

	/**
	 * Privilege to hook input.
	**/
	bool hookinput:1;

	/**
	 * Access to the overlay.
	**/
	bool overlay:1;

	/**
	 * Direct rendering prvilege.
	**/
	bool render:1;

	/**
	 * Access to metamodding.
	**/
	bool metamod:1;
};

/**
 * Represents a Sandblox mod.
**/
struct Mod{
	/**
	 * A list of all loaded mods.
	**/
	static std::vector<Mod> mods;

	/**
	 * The mod's name.
	**/
	std::string name;
	/**
	 * The mod's version.
	**/
	Version* v;
	/**
	 * A list of authors.
	**/
	std::vector<std::string> authors;
	/**
	 * The type of mod.
	**/
	enum ModType{
		PACKAGE,
		WORLD,
		GENERATOR,
		META,
		VIRAL,
		CONTENT,
		RESOURCE
	}type;
	/**
	 * The permissions of the mod.
	**/
	Permissions permissions;
	/**
	 * Array of resources.
	**/
	std::vector<Resource> resources;
	/**
	 * Array of included modules/libraries.
	**/
	std::vector<PyObject*> libs;
	/**
	 * The mods this mod is able to metamod (0th entry is always this).
	**/
	std::vector<std::string> metamods;

	/**
	 * The mod's interpreter environment, containing all available globals initialized to this mod's context.
	**/
	PyObject* environment;

	/**
	 * Build a Mod object from the given folder.
	 *
	 * @param fname The folder's name.
	**/
	Mod(const std::string& fname);

	/**
	 * Returns if this mod has access to the given mod.
	 *
	 * @param m The mod to check for access.
	**/
	bool can_metamod(Mod* m);

	/**
	 * Compile a mod into a .mod file for easy sharing.
	 *
	 * @param f The mod's path.
	 * @param out The output file's name.
	**/
	static void compile(const std::string& f,const std::string& out);

	/**
	 * marshal.dumps, pointer filled by init_mods()
	 *
	 * Preloaded for speed.
	**/
	static PyObject* marshal_dumps;

	/**
	 * marshal.loads, pointer filled by init_mods()
	 *
	 * Preloaded for speed.
	**/
	static PyObject* marshal_loads;
};

struct Section{
	/**
	 * Section signature
	**/
	char signature[4];
	/**
	 * The length in bytes of the chunk (after this)
	**/
	endian::ulil_t chunklength;
};

/**
 * The mod section of a .mod file
**/
struct ModSection{
	/**
	 * The 4-byte version number.
	**/
	endian::ulil_t version;
	/**
	 * The type of mod
	 *
	 * Devnote: Is this needed?
	**/
	endian::ulil8_t type;
	/**
	 * A bit field of permissions.
	 *
	 * 0 - read world
	 * 1 - write to world
	 * 2 - player access
	 * 3 - input hooking
	 * 4 - overlay rendering
	 * 5 - 3d rendering
	**/
	endian::ulil8_t permission;
	/**
	 * The mod's name.
	**/
	std::string name;
	/**
	 * The mod's description.
	**/
	std::string description;
	/**
	 * A list of mod authors.
	**/
	std::vector<std::string> authors;
	/**
	 * A list of mods it's metamodding.
	**/
	std::vector<std::string> metamods;

	/**
	 * Writes the section's contents to the given file
	 *
	 * @param f The FILE pointer to the file opened for writing.
	**/
	void write(FILE* f);
};

/**
 * The resource section of a .mod file
**/
struct ResourceSection{
	/**
	 * A list of resources.
	**/
	std::vector<std::string> resources;

	/**
	 * Writes the section's contents to the given file
	 *
	 * @param f The FILE pointer to the file opened for writing.
	**/
	void write(FILE* f);
};

/**
 * The code section of a .mod file
**/
struct CodeSection{
	/**
	 * A list of Python code objects.
	**/
	std::vector<PyObject*> codebits;

	/**
	 * Writes the section's contents to the given file
	 *
	 * @param f The FILE pointer to the file opened for writing.
	**/
	void write(FILE* f);
};

/**
 * The block definition section of the .mod file
**/
struct BloxSection{
	/**
	 * The basic block definition structure.
	**/
	struct BlockDef{
		/**
		 * The block's name.
		**/
		std::string name;
		/**
		 * The block's description.
		**/
		std::string description;
		/**
		 * The amount of light the block produces (usually 0.0)
		**/
		float light;
		/**
		 * Gravity's effect on the block (usually 0.0 - 1.0 is standard gravity)
		**/
		float gravity;
		/**
		 * The filename of the code object providing extra functionality to the block.
		**/
		std::string code;
		/**
		 * The filename of the mesh.
		 *
		 * Devnote: Since these are mostly going to be cubes, look into ID'd meshes
		**/
		std::string mesh;
		/**
		 * The skin of the mesh.
		 *
		 * Devnote: Can these files be arithmetically ID'd?
		**/
		std::string skin;

		/**
		 * Writes the block definition's contents to the given file
		 *
		 * @param f The FILE pointer to the file opened for writing.
		**/
		void write(FILE* f);

		/**
		 * Loads a block definition from the given xml file.
		 *
		 * @param fname The file's name.
		**/
		void load(const std::string& fname);
	};

	/**
	 * The list of block definitions.
	**/
	std::vector<BlockDef*> blockdefs;

	/**
	 * Writes the section's contents to the given file
	 *
	 * @param f The FILE pointer to the file opened for writing.
	**/
	void write(FILE* f);

	~BloxSection(){
		for(unsigned i=0;i<blockdefs.size();++i){
			delete blockdefs[i];
		}
	}
};

#endif
