#ifndef SANDBLOX_GAME_H
#define SANDBLOX_GAME_H

#include <irrlicht.h>
#include <SColor.h>

#include <vector>

#include "error.h"
/*#include "mod.h"
#include "chunk.h"*/
#include "input.h"
#include "player.h"

using namespace irr;

/**
 * Container/abstraction class to handle things related to the game.
**/
class Game{
	protected:
		/**
		 * The window's width.
		**/
		static const u32 WIDTH;
		/**
		 * The window's height.
		**/
		static const u32 HEIGHT;
		/**
		 * The window's initial title.
		**/
		static const wchar_t* TITLE;
		/**
		 * The color to clear the window to.
		**/
		static const video::SColor CLEARCOLOR;

	public:
		IrrlichtDevice* device;
		video::IVideoDriver* driver;
		scene::ISceneManager* scenemanager;
		gui::IGUIEnvironment* guienv;

		Player player;

		/*vector<Mod> mods;
		Chunk chunk;*/

		/**
		 * Game input.
		**/
		Input input;

		/**
		 * Flag used to exit the game, especially when you're out of the main loop's call stack.
		**/
		bool end;

		~Game();

		//Functions are intended to be abstracted in case a new engine is required.

		/**
		 * Initialize the game (put into a non-constructor function to give more control).
		**/
		void init();
		/**
		 * Draw everything to the screen.
		**/
		void draw();
		/**
		 * Eat away CPU cycles when they're not needed.
		**/
		void yield();
		/**
		 * Draw text to the screen.
		**/
		void draw_text(const wchar_t*,unsigned x1,unsigned y1,unsigned x2,unsigned y2);

		/**
		 * @return Whether or not the window is still running.
		**/
		bool running();
		/**
		 * @return Whether or not the window is being actively viewed by the player.
		**/
		bool isactive();
};

extern Game game;

#endif
