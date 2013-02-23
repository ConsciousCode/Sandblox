#ifndef SANDBLOX_MOD_H
#define SANDBLOX_MOD_H

/**
 * Initialize the game module.
**/
void init_gamemod();

/**
 * Initialize the render module.
**/
void init_rendermod();

/**
 * Initialize the mod module.
**/
void init_modmod();

/**
 * Initialize Python modules and mod-handling routines.
**/
void init_mods();

/**
 * Abstract representation of a resource.
**/
struct Resource{

};

/**
 * Structure of mod privileges.
**/
struct Privileges{
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
	 * The mod's name.
	**/
	char* name;
	/**
	 * Array of resources.
	**/
	Resource* resources;
	/**
	 * Array of included modules/libraries.
	**/
	PyObject** libs;
	/**
	 * The privileges of the mod.
	**/
	Privileges privileges;
	/**
	 * The mods this mod is able to metamod (0th entry is always this).
	**/
	Mod* metamods;

	/**
	 * Returns if this mod has access to the given mod.
	 *
	 * @param m The mod to check for access.
	**/
	bool can_metamod(Mod* m);
};

#endif
