/**
 * Include file bringing everything together.s
**/
#ifndef SANDBLOX_INCLUDE_H
#define SANDBLOX_INCLUDE_H

#include <irrlicht.h>

#ifdef _IRR_WINDOWS_
	#define WINDOWS
#endif

#define SANDBLOX_VERSION "1.0.0.0"
#define SANDBLOX_3DENGINE "Irrlicht"

#include "game.h"

#include "error.h"
#include "block.h"
//#include "color.h"
//#include "map.h"
//#include "mod.h"

#endif
