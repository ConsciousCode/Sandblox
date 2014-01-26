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
 * @file game/game.hpp
 *
 * The core game class that ties everything together.
**/

#pragma once

#ifndef SANDBLOX_GAME_GAME
#define SANDBLOX_GAME_GAME

#include <string>

#include "../engine/include.hpp"

class Player;
class Game;

#include "player.hpp"

class Game:public EventReceiver{
	public:
		/**
		 * The engine encapsulates the entire 3D engine.
		**/
		Engine* engine;
		/**
		 * An object used to represent the 3D scene.
		**/
		Scene* scene;

		/**
		 * The player object manages things specific to the player
		 * (camera, inventory, health, etc).
		**/
		Player* player;

		bool keys[ENGINE_NUM_KEYS];
		bool ml,mm,mr;
		unsigned mx,my;
		float delta;

		Game(const std::string& title);

		~Game();

		/**
		 * Run the entire game until it's exited.
		**/
		void run();

		bool OnMouse(Event e);
		bool OnKey(Event e);
};

#endif
