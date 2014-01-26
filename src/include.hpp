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
 * @file include.hpp
 *
 * Base include file.
**/

#pragma once

#ifndef SANDBLOX_INCLUDE
#define SANDBLOX_INCLUDE

#include "config.hpp"

#include "game/include.hpp"
#include "engine/include.hpp"

/**
 * Initialize the game and all its components.
**/
Game* init();

/**
 * Deinitialize the game and all its components.
**/
void deinit(Game* game);

#endif
