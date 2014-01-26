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
 * @file game/block.hpp
 *
 * Defines block definitions.
**/

#pragma once

#ifndef SANDBLOX_GAME_BLOCK
#define SANDBLOX_GAME_BLOCK

#include <string>

class Block{
	public:
		/**
		 * The name of the block.
		**/
		std::string name;
		/**
		 * A path to the block's texture.
		**/
		std::string texture;

		Block(const std::string& name,const std::string& texture);
};

#endif
