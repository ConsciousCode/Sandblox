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
 * @file game/game.hpp
 *
 * The core game class that ties everything together.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_EVENT
#define SANDBLOX_ENGINE_EVENT

#include "../config.hpp"

class Event{
	public:
		unsigned which;
		bool what;
		float delta;
		unsigned mx,my;
};

const unsigned
	LMOUSE=0,
	MMOUSE=1,
	RMOUSE=2,
	MOUSE_MOVE=3,
	MOUSE_WHEEL=4;

class EventReceiver{
	public:
		virtual ~EventReceiver(){}

		virtual bool OnMouse(Event e)=0;
		virtual bool OnKey(Event e)=0;
};

#endif
