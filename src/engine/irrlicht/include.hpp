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
 * @file engine/irrlicht/include.hpp
 *
 * Include everything for Irrlicht.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_IRRLICHT_INCLUDE
#define SANDBLOX_ENGINE_IRRLICHT_INCLUDE

#include "ievent.hpp"
#include "iengine.hpp"

class Game;
#include "../../game/game.hpp"

Engine* irrlicht_init(Game* g);

void irrlicht_deinit(Engine* e);

#endif
