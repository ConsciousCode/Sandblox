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
 * @file game/include.hpp
 *
 * Base game include file.
**/

#pragma once

#ifndef SANDBLOX_GAME_INCLUDE
#define SANDBLOX_GAME_INCLUDE

#include "game.hpp"
#include "block.hpp"

#include "../engine/engine.hpp"

Game* game_init();

void game_deinit(Game* g);

#endif
