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

#include "icamera.hpp"

IrrlichtScene::IrrlichtScene(irr::video::IVideoDriver* d,irr::scene::ISceneManager* s):driver(d),scene(s){
	d->grab();
	s->grab();
}

IrrlichtScene::~IrrlichtScene(){
	driver->drop();
	scene->drop();
}

void IrrlichtScene::draw(){
	driver->beginScene(true,true,irr::video::SColor(0,128,30,30));
	scene->drawAll();
	driver->endScene();
}

void IrrlichtScene::add_block(Block* b){
	irr::scene::ISceneNode* n=scene->addCubeSceneNode();
	n->setMaterialType(irr::video::EMT_SOLID);
	printf("Driver: 0x%p\nBlock: 0x%p\nPath: %s\n",driver,b,b->texture.c_str());
	irr::video::ITexture* p=driver->getTexture(irr::io::path(irr::core::string<char>(b->texture.c_str())));
	printf("Returned: 0x%p",p);
	if(p){
		n->setMaterialTexture(0,p);
	}
	printf("Loaded texture\n");
	n->setMaterialFlag(irr::video::EMF_LIGHTING,false);
	n->setPosition(irr::core::vector3df(0,0,50));
}

Camera* IrrlichtScene::make_camera(){
	return new IrrlichtCamera(scene);
}

unsigned IrrlichtScene::get_width(){
	return driver->getScreenSize().Width;
}

unsigned IrrlichtScene::get_height(){
	return driver->getScreenSize().Height;
}
