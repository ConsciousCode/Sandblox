#include <irrlicht.h>

#include "player.h"
#include "game.h"

using namespace irr;

/*
 * PlayerCamera methods
*/

void PlayerCamera::init(){
	camera=game.scenemanager->addCameraSceneNode(0,core::vector3df(0,30,-40),core::vector3df(0,5,0));
}

void PlayerCamera::move(float x,float y,float z){
	camera->setPosition(core::vector3df(x,y,z)+camera->getPosition());
}

void PlayerCamera::moveto(float x,float y,float z){
	camera->setPosition(core::vector3df(x,y,z));
}

/*
 * Player methods
*/

void Player::init(){
	camera.init();
}
