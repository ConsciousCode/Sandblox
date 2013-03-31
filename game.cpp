#include <irrlicht.h>
#include <SColor.h>

#include <vector>

#include "error.h"
/*#include "mod.h"
#include "chunk.h"*/

#include "game.h"
#include "input.h"

using namespace irr;

/*
 * Game static members
*/

const wchar_t* Game::TITLE=L"Sandblox";
const video::SColor Game::CLEARCOLOR(255,255,0,0);

const u32 Game::WIDTH=640;
const u32 Game::HEIGHT=480;

/*
 * Input callback functions
*/

/**
 * Hook function for the ESC key.
**/
static PyObject* esc_hook(PyObject*,PyObject*){
	game.end=true;

	///TODO: Add cleanups?
}

/*
 * Game methods
*/

Game game;

Game::~Game(){
	driver->drop();
}

void Game::init(){
	//Irrlicht initialization
	device=createDevice(
		video::EDT_SOFTWARE,core::dimension2d<u32>(WIDTH,HEIGHT),
		16,false,false,false,&input
	);
	if(!device){
		throw InitError("Irrlicht device could not be created.");
	}
	device->setWindowCaption(TITLE);

	driver=device->getVideoDriver();
	scenemanager=device->getSceneManager();
	guienv=device->getGUIEnvironment();

	//Input hooks
	input.hooks.push_back(InputHook(NULL,KEY,KEY_ESCAPE,esc_hook));

	//Player init
	player.init();
}

void Game::draw(){
	driver->beginScene(true,true,CLEARCOLOR);

	scenemanager->drawAll();
	guienv->drawAll();

	driver->endScene();
}

void Game::yield(){
	device->yield();
}

void Game::draw_text(const wchar_t* text,unsigned x1,unsigned y1,unsigned x2,unsigned y2){
	guienv->addStaticText(text,core::rect<s32>(x1,y1,x2,y2));
}

bool Game::running(){
	return device->run();
}

bool Game::isactive(){
	return device->isWindowActive();
}
