#ifndef SANDBLOX_MOD_H
#define SANDBLOX_MOD_H

#include <Python.h>

#include <string>

#include "endian.h"

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
struct Resource{};

/**
 * Structure of mod privileges.
**/
struct Permissions{
	//Access privileges

	/**
	 * Read/write access to the map.
	**/
	bool readmap,writemap;

	/**
	 * Read/write access to the player.
	**/
	bool player;

	/**
	 * Privilege to hook input.
	**/
	bool hookinput;

	/**
	 * Access to the overlay.
	**/
	bool overlay;

	/**
	 * Direct rendering prvilege.
	**/
	bool render;

	/**
	 * Access to metamodding.
	**/
	bool metamod;
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
	std::vector<Mod*> metamods;

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
	 * @param fname The mod's path.
	 * @param out The output file's name.
	**/
	static void compile(const std::string& folder,const std::string& out);
};

/**
 * The mod section of a .mod file
**/
struct ModSection{
	/**
	 * "MOD"
	**/
	char signature[3];
	/**
	 * The length in bytes of the chunk (after this)
	**/
	ulil_t chunklength;
	/**
	 * The 4-byte version number.
	**/
	ulil_t version;
	/**
	 * The type of mod
	 *
	 * Devnote: Is this needed?
	**/
	ulil8_t type;
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
	ulil8_t permission;
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
	 * "EMBED"
	**/
	char signature[5];
	/**
	 * The length in bytes of the chunk (after this)
	**/
	ulil_t chunklength;
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
	 * "CODE"
	**/
	char signature[4];
	/**
	 * The length in bytes of the chunk (after this)
	**/
	ulil_t chunklength;
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
	 * "BLOX"
	**/
	char signature[4];
	/**
	 * The length in bytes of the chunk (after this)
	**/
	ulil_t chunklength;

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
};

/**
 * marshal.dumps, pointer filled by init_mods()
 *
 * Preloaded for speed.
**/
PyObject* marshal_dumps;

/**
 * marshal.loads, pointer filled by init_mods()
 *
 * Preloaded for speed.
**/
PyObject* marshal_loads;

#endif
