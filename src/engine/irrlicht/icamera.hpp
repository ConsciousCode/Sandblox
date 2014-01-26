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
 * @file engine/irrlicht/icamera.hpp
 *
 * A class encapsulating Irrlicht's Camera Scene Node.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_IRRLICHT_ICAMERA
#define SANDBLOX_ENGINE_IRRLICHT_ICAMERA

#include <irrlicht.h>

#include "iscene.hpp"

#include "../camera.hpp"

class IrrlichtCamera:public Camera{
	protected:
		irr::scene::ICameraSceneNode* camera;
	public:
		IrrlichtCamera(irr::scene::ISceneManager* scene);

		virtual ~IrrlichtCamera(){}

		virtual void move(float x,float y,float z);
		virtual void translate(float x,float y,float z);
		virtual void rotate(float z,float y,float x=0.0f);
};

#endif
