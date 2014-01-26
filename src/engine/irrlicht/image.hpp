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
 * @file engine/irrlicht/image.hpp
 *
 * Irrlicht's image wrapper overload.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_IRRLICHT_IMAGE
#define SANDBLOX_ENGINE_IRRLICHT_IMAGE

#include <IImage.h>

#include "../image.hpp"

class IrrlichtImage:public Image{
	protected:
		irr::video::IImage image;
	public:
		//???
};

#endif
