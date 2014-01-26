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
 * @file engine/ievent.hpp
 *
 * Defines the irrlicht-specific event handling stuff.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_IRRLICHT_EVENT
#define SANDBLOX_ENGINE_IRRLICHT_EVENT

#include <irrlicht.h>

#include "../event.hpp"

class IrrlichtEventProxy:public irr::IEventReceiver{
	public:
		IrrlichtEventProxy(EventReceiver* r);

		EventReceiver* eventr;

		bool OnEvent(const irr::SEvent& e);
};

const unsigned ENGINE_NUM_KEYS=irr::KEY_KEY_CODES_COUNT;
const unsigned ENGINE_ESC_KEY=irr::KEY_ESCAPE;

#endif
