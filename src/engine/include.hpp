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
 * @file engine/include.hpp
 *
 * Include whatever will be used for the 3D rendering engine.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_INCLUDE
#define SANDBLOX_ENGINE_INCLUDE

#if SANDBLOX_ENGINE==IRRLICHT_ENGINE
	#include "irrlicht/include.hpp"
#endif

#include "engine.hpp"

#include "../config.hpp"

class Game;
#include "../game/game.hpp"

/**
 * Initialize the 3D engine.
**/
Engine* engine_init(Game* g);

void engine_deinit(Engine* e);

#endif
