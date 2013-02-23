#include <irrlicht.h>

#include "block.h"

using namespace irr;

Block::Block(ISceneNode* parent):Object3D(parent){
	material.Wireframe=false;
	material.Lighting=false;

	vertices[0] = video::S3DVertex(0,0,10, 1,1,0,
                video::SColor(255,0,255,255), 0, 1);
        vertices[1] = video::S3DVertex(10,0,-10, 1,0,0,
                video::SColor(255,255,0,255), 1, 1);
        vertices[2] = video::S3DVertex(0,20,0, 0,1,1,
                video::SColor(255,255,255,0), 1, 0);
        vertices[3] = video::S3DVertex(-10,0,-10, 0,0,1,
                video::SColor(255,0,255,0), 0, 0);

	box.reset(vertices[0].Pos);
        for (s32 i=1; i<4; ++i)
            box.addInternalPoint(vertices[i].Pos);
}

void Block::OnRegisterSceneNode(){
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void Block::render(){
	u16 i[] = {   0,2,3, 2,1,3, 1,0,3, 2,0,1  };

	game.driver->setMaterial(material);
	game.driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	game.driver->drawVertexPrimitiveList(&vertices[0], 4, &i[0], 4, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
}

const core::aabbox3d<f32>& Block::getBoundingBox() const{
	return box;
}

