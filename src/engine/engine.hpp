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
 * @file engine/engine.hpp
 *
 * Defines a generic wrapper class for the 3D engine.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_ENGINE
#define SANDBLOX_ENGINE_ENGINE

#include "scene.hpp"
#include "event.hpp"

/**
 * A class wrapping the internals of the underrlying 3D engine.
**/
class Engine{
	public:
		/**
		 * The 3D scene.
		**/
		Scene* scene;

		virtual ~Engine(){}

		/**
		 * @return whether or not the engine is still running.
		**/
		virtual bool running()=0;

		/**
		 * Set the window's title to the given string.
		**/
		virtual void set_title(const std::string& s)=0;

		/**
		 * Set the engine's input manager.
		**/
		virtual void set_event(EventReceiver* eventr)=0;

		/**
		 * Stop the game.
		**/
		virtual void stop()=0;
};

#endif
