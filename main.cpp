#include <Python.h>
#include "include.h"

int main(){
	Mod::compile("../../defmod/","../../default.mod");
	#if 0
	try{
		//initialize
		Py_Initialize();
		game.init();

		//game.scenemanager->addCameraSceneNodeFPS();//0,core::vector3df(0,-40,0),core::vector3df(0,0,0));

		//Block* block=new Block(game.scenemanager->getRootSceneNode());
		/*scene::ISceneNodeAnimator* anim=game.scenemanager->createRotationAnimator(core::vector3df(0.8f,0,0.8f));
		if(anim){
			block->addAnimator(anim);
			anim->drop();
			anim=NULL;
		}*/
		//block->drop();
		//block=NULL;

		/*init_hooks();
		init_mods();*/

		while(game.running() and not game.end){
			if(game.isactive()){
				//game.player.move();

				/*for(unsigned i=0;i<game.mods.size();++i){
					game.mods[i].run();
				}*/

				//game.chunk.draw();

				//Put it all on the screen
				game.draw();
			}
			else{
				game.yield();
			}
		}
	}
	catch(const EndGameError& e){}
	catch(const HookError& e){
		/* print error */
	}

	return 0;
	#endif
}
