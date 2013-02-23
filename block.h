#ifndef SANDBLOX_BLOCK_H
#define SANDBLOX_BLOCK_H

#include <irrlicht.h>

#include "object3d.h"

using namespace irr;

/**
 * A class representing a basic block component.
**/
struct Block:public Object3D{
	protected:
		core::aabbox3d<f32> box;//?
		video::S3DVertex vertices[4];
		video::SMaterial material;
	public:
		Block(scene::ISceneNode*);

		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		virtual void OnRegisterSceneNode();

		virtual void render();
};

#endif
