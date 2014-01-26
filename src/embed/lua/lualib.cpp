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

#include "lualib.hpp"
#include "util.hpp"

void lualib_init(LuaVM* vm){
	lua_State* L=vm->_state;

	luaL_requiref(L,"",luaopen_base,1);
	luaL_requiref(L,"string",luaopen_string,1);
	luaL_requiref(L,"math",luaopen_math,1);
	luaL_requiref(L,"package",luaopen_package,1);
}
