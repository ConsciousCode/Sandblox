/*
This file is part of Sandblox.

	Sandblox is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Sandblox is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Sandblox.  If not, see <http://www.gnu.org/licenses/>.
*/

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
//void load_mods();

/**
 * Abstract representation of a resource.
**/
struct Resource{
	std::string name;

	unsigned length;
	unsigned char* data;

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
	struct authorcontrib{
		std::string author;
		std::string contribution;
	};

	/**
	 * A list of all loaded mods.
	**/
	static std::vector<Mod> mods;

	/**
	 * The last error
	**/
	std::string error;

	/**
	 * The mod's name.
	**/
	std::string name;
	/**
	 * The mod's description.
	**/
	std::string description;
	/**
	 * The mod's version.
	**/
	Version* v;
	/**
	 * A list of authors.
	**/
	std::vector<authorcontrib> authors;
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
	 * Compile a mod into a .mod file for easy sharing.
	 *
	 * @param f The mod's path.
	 * @param out The output file's name.
	**/
	bool compile(const std::string& f,const std::string& out);

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

#endif
