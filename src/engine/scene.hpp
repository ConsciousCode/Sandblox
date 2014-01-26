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
 * @file engine/screen.hpp
 *
 * Generic wrapper for the screen.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_SCENE
#define SANDBLOX_ENGINE_SCENE

#include "../game/block.hpp"
#include "camera.hpp"

class Scene{
	public:
		virtual ~Scene(){}

		/**
		 * Draw the scene.
		**/
		virtual void draw()=0;

		/**
		 * Add the given block to the scene.
		**/
		virtual void add_block(Block* b)=0;

		/**
		 * Create a camera for the scene.
		**/
		virtual Camera* make_camera()=0;

		/**
		 * Get the width of the window.
		**/
		virtual unsigned get_width()=0;

		/**
		 * Get the height of the window.
		**/
		virtual unsigned get_height()=0;
};

#endif
