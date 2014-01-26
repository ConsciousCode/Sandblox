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

#include "icamera.hpp"

IrrlichtCamera::IrrlichtCamera(irr::scene::ISceneManager* scene){
	camera=scene->addCameraSceneNode();
	camera->bindTargetAndRotation(true);
}

void IrrlichtCamera::move(float x,float y,float z){
	camera->setPosition(irr::core::vector3df(x,y,z));
}

void IrrlichtCamera::translate(float x,float y,float z){
	irr::core::vector3df v=camera->getPosition();
	camera->setPosition(irr::core::vector3df(v.X+x,v.Y+y,v.Z+z));
}

void IrrlichtCamera::rotate(float z,float y,float x){
	irr::core::vector3df v=camera->getRotation();
	camera->setRotation(irr::core::vector3df(v.X+x*360,v.Y+y*360,v.Z+z*360));
}
