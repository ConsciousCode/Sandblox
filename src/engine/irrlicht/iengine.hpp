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
 * @file engine/irrlicht/irrlictengine.hpp
 *
 * Defines a 3D library wrapper for Irrlicht.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_IRRLICHT_IRRLICHTENGINE
#define SANDBLOX_ENGINE_IRRLICHT_IRRLICHTENGINE

#include <irrlicht.h>

#include "ievent.hpp"

#include "../engine.hpp"

#include "../../config.hpp"

class Game;
#include "../../game/game.hpp"

class IrrlichtEngine:public Engine{
	protected:
		irr::IrrlichtDevice* device;
		IrrlichtEventProxy* proxy;
	public:
		IrrlichtEngine(Game* g,unsigned w,unsigned h,irr::video::E_DRIVER_TYPE t=irr::video::EDT_OPENGL);

		virtual ~IrrlichtEngine();

		virtual bool running();
		virtual void set_title(const std::string& s);
		virtual void set_event(EventReceiver* eventr);
		virtual void stop();
};

#endif
