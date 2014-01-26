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

#include "iscene.hpp"
#include "iengine.hpp"

#include "../../game/game.hpp"

#include "../../util.hpp"
using namespace util;

IrrlichtEngine::IrrlichtEngine(Game* g,unsigned w,unsigned h,irr::video::E_DRIVER_TYPE t){
	device=irr::createDevice(t,irr::core::dimension2d<irr::u32>(w,h));
	scene=new IrrlichtScene(device->getVideoDriver(),device->getSceneManager());
	device->getCursorControl()->setVisible(false);
	proxy=new IrrlichtEventProxy(g);
	print("FUUUUUUUUUCK",proxy);
	device->setEventReceiver(proxy);
	print("Success!");
}

IrrlichtEngine::~IrrlichtEngine(){
	delete scene;
	delete proxy;
	device->drop();
}

bool IrrlichtEngine::running(){
	return device->run();
}

void IrrlichtEngine::set_title(const std::string& s){
	device->setWindowCaption(std::wstring(s.begin(),s.end()).c_str());
}

void IrrlichtEngine::set_event(EventReceiver* eventr){
	proxy->eventr=eventr;
}

void IrrlichtEngine::stop(){
	device->closeDevice();
}
