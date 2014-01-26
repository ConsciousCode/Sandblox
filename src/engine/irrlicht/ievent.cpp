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

#include "ievent.hpp"
#include <cstdio>

IrrlichtEventProxy::IrrlichtEventProxy(EventReceiver* r){
	eventr=r;
}

bool IrrlichtEventProxy::OnEvent(const irr::SEvent& ie){
	if(ie.EventType==irr::EET_MOUSE_INPUT_EVENT){
		Event e;
		switch(ie.MouseInput.Event){
			case irr::EMIE_LMOUSE_PRESSED_DOWN:
				e.which=LMOUSE;
				e.what=true;
				break;
			case irr::EMIE_LMOUSE_LEFT_UP:
				e.which=LMOUSE;
				e.what=false;
				break;
			case irr::EMIE_MMOUSE_PRESSED_DOWN:
				e.which=MMOUSE;
				e.what=true;
				break;
			case irr::EMIE_MMOUSE_LEFT_UP:
				e.which=MMOUSE;
				e.what=false;
				break;
			case irr::EMIE_RMOUSE_PRESSED_DOWN:
				e.which=RMOUSE;
				e.what=true;
				break;
			case irr::EMIE_RMOUSE_LEFT_UP:
				e.which=RMOUSE;
				e.what=false;
				break;
			case irr::EMIE_MOUSE_MOVED:
				e.which=MOUSE_MOVE;
				e.mx=ie.MouseInput.X;
				e.my=ie.MouseInput.Y;
				break;
			case irr::EMIE_MOUSE_WHEEL:
				e.which=MOUSE_WHEEL;
				e.delta=ie.MouseInput.Wheel;
				break;
			default:
				break;
		}
		return eventr->OnMouse(e);
	}
	else if(ie.EventType==irr::EET_KEY_INPUT_EVENT){
		Event e;
		e.which=ie.KeyInput.Key;
		e.what=ie.KeyInput.PressedDown;
		return eventr->OnKey(e);
	}
	else{
		return false;
	}
}
