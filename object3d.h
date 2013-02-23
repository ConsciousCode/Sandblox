#ifndef SANDBLOX_OBJECT3D_H
#define SANDBLOX_OBJECT3D_H

#include <irrlicht.h>

#include "game.h"

using namespace irr;

struct Object3D:public scene::ISceneNode{
	Object3D(scene::ISceneNode* parent):ISceneNode(parent,game.scenemanager,0){}
};

#endif
