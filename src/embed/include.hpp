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
 * @file embed/include.hpp
 *
 * Include file for the embedded language(s)
**/

#pragma once

#ifndef SANDBLOX_EMBED_INCLUDE
#define SANDBLOX_EMBED_INCLUDE

#include "../config.hpp"
#include "vm.hpp"

#if SANDBLOX_LANG==LUA_LANG
	#include "lua/include.hpp"
#endif

/**
 * Initialize the embedded language.
**/
VM* embed_init();

/**
 * Deinitialize the embedded language.
**/
void embed_deinit(VM* vm);

#endif
