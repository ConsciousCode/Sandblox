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
 * @file engine/irrlicht/node.hpp
 *
 * Irrlicht's Node class.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_IRRLICHT_NODE
#define SANDBLOX_ENGINE_IRRLICHT_NODE

#include <irrlicht.h>

#include "../node.hpp"

class IrrlichtNode{
	protected:
		irr::scene::ISceneNode* node;
	public:
		IrrlichtNode(Scene scene,Block* b){
			node=((IrrlichtNode)scene.add_cube(1)).get_internal();
			node->setMaterialFlag(irr::video::EMF_LIGHTING,false);
			node->setMaterialTexture(0,"resources/ponies.jpg");
		}

		irr::scene::IsceneNode* get_internal(){
			return node;
		}
};

#endif