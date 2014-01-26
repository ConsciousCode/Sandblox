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
 * @file embed/lua/luavm.hpp
 *
 * Defines the Lua subset of VM.
**/

#pragma once

#ifndef SANDBLOX_EMBED_LUA_LUAVM
#define SANDBLOX_EMBED_LUA_LUAVM

#include <lua.hpp>

#include "../vm.hpp"

/**
 * A wrapper VM for Lua.
**/
class LuaVM:public VM{
	public:
		/**
		 * The internal object reprsenting Lua's VM state.
		**/
		lua_State* _state;

		LuaVM();
		~LuaVM();
};

#endif
