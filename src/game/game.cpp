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

#include "game.hpp"

Game::Game(const std::string& title):engine(engine_init(this)),scene(engine->scene){
	player=new Player(this);
	engine->set_title(title.c_str());

	engine->set_event(this);
}

Game::~Game(){
	delete player;
	engine_deinit(engine);
}

void Game::run(){
	while(engine->running()){
		scene->draw();
	}
}

bool Game::OnMouse(Event e){
	if(e.what==MOUSE_MOVE){
		mx=e.mx;
		my=e.my;
		player->camera->rotate((1-mx/scene->get_width()/2)*0.01f,(1-my/scene->get_height()/2)*0.01f);
	}
	else if(e.which==LMOUSE){
		ml=e.what;
	}
	else if(e.which==MMOUSE){
		mm=e.what;
	}
	else if(e.which==MOUSE_WHEEL){
		delta=e.delta;
	}
	else if(e.which==RMOUSE){
		mr=e.what;
	}

	return true;
}

#include <irrlicht.h>

bool Game::OnKey(Event e){
	if(e.which==ENGINE_ESC_KEY){
		engine->stop();
	}
	else if(e.which==irr::KEY_KEY_A){
		player->camera->translate(0,-0.1,0);
	}
	else if(e.which==irr::KEY_KEY_D){
		player->camera->translate(0,0.1,0);
	}
	else if(e.which==irr::KEY_KEY_W){
		player->camera->translate(0.1,0,0);
	}
	else if(e.which==irr::KEY_KEY_S){
		player->camera->translate(-0.1,0,0);
	}

	keys[e.which]=e.what;

	return true;
}
