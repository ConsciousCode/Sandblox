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
 * @file embed/lua/include.hpp
 *
 * The include file for all Lua-related code.
**/

#pragma once

#ifndef SANDBLOX_EMBED_LUA_INCLUDE
#define SANDBLOX_EMBED_LUA_INCLUDE

#include <lua.hpp>

#include "../vm.hpp"
#include "lualib.hpp"
#include "luavm.hpp"

/**
 * Initialize a Lua machine.
**/
LuaVM* lua_init();

/**
 * Deinitialize a Lua machine.
**/
void lua_deinit(VM* vm);

#endif
