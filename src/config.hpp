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
	along with Sandblox. If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file config.hpp
 *
 * Configure Sandblox settings.
**/

#pragma once

#ifndef SANDBLOX_CONFIG
#define SANDBLOX_CONFIG

//General config
#define SANDBLOX_START_W 640
#define SANDBLOX_START_H 480
#define SANDBLOX_TITLE "Gotta love sand!"

#define EVAL_CONCAT(x,y) x ## y
#define CONCAT_MACRO(x,y) EVAL_CONCAT(x,y)

///Irrlicht stuff
#define IRRLICHT_ENGINE 1
#define IRRLICHT_NAME irrlicht
#define IRRLICHT_VERSION 1.8

///Lua stuff
#define LUA_LANG 1
#define LUA_NAME lua
#define LUA_VERSION 5.2

///Sandblox stuff
#define SANDBLOX_WHICH_ENGINE IRRLICHT
#define SANDBLOX_WHICH_LANG LUA

//Engine macros
#define SANDBLOX_ENGINE CONCAT_MACRO(SANDBLOX_WHICH_ENGINE,_ENGINE)
#define SANDBLOX_ENGINE_VERSION CONCAT_MACRO(SANDBLOX_WHICH_ENGINE,_VERSION)
#define SANDBLOX_ENGINE_INIT CONCAT_MACRO(CONCAT_MACRO(SANDBLOX_WHICH_ENGINE,_NAME),_init)
#define SANDBLOX_ENGINE_DEINIT CONCAT_MACRO(CONCAT_MACRO(SANDBLOX_WHICH_ENGINE,_NAME),_deinit)

//Language macros
#define SANDBLOX_LANG CONCAT_MACRO(SANDBLOX_WHICH_LANG,_LANG)
#define SANDBLOX_LANG_VERSION CONCAT_MACRO(SANDBLOX_WHICH_LANG,_VERSION)
#define SANDBLOX_LANG_INIT CONCAT_MACRO(CONCAT_MACRO(SANDBLOX_WHICH_LANG,_NAME),_init)
#define SANDBLOX_LANG_DEINIT CONCAT_MACRO(CONCAT_MACRO(SANDBLOX_WHICH_LANG,_NAME),_deinit)

#endif
