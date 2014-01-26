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
 * @file game/map.hpp
 *
 * Defines the main Sandblox map.
**/

#pragma once

#ifndef SANDBLOX_GAME_MAP
#define SANDBLOX_GAME_MAP

#include "block.hpp"

class Octree{
	protected:
		/**
		 * The data for the octree.
		**/
		Octree* blocks[8];
	public:
		/**
		 * The id of the block, -1 if it's subdivided.
		**/
		short id;

		/**
		 * Get the block at the given position (0-7).
		 * The order of the blocks is (-,-,-), (-,+,-), (+,+,-), (+,-,-), (-,-,+), (-,+,+), (+,+,+), (+,-,+)
		**/
		Block*& operator[](unsigned x);
};

class Map{
	protected:
		/**
		 * List of block definitions used for indexing.
		**/
		std::vector<Block*> sand;

		/**
		 * The coordinates of the center of the octree block.
		**/
		unsigned cx,cy,cz;
		/**
		 * A zoom value to figure out the scale.
		**/
		unsigned zoom;

		/**
		 * The loaded octree of the map.
		**/
		Octree* loaded;
	public:
		/**
		 * Get the block definition with the given id.
		**/
		Block* block_def(short id);
};

#endif
