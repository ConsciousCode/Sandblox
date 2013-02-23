#ifndef SANDBLOX_PLAYER_H
#define SANDBLOX_PLAYER_H

#include <irrlicht.h>

using namespace irr;

/**
 * Wrapper around ICameraSceneNode for use in Player.
**/
struct PlayerCamera{
	/**
	 * The internal pointer.
	**/
	scene::ICameraSceneNode* camera;

	/**
	 * Initialize the camera.
	**/
	void init();
	/**
	 * Move the camera by a certain amount.
	 *
	 * @param x How far the camera should move in the x direction.
	 * @param y How far the camera should move in the y direction.
	 * @param z How far the camera should move in the z direction.
	**/
	void move(float x,float y,float z);
	/**
	 * Move the camera to a specified location.
	 *
	 * @param x The x component of the position.
	 * @param y The y component of the position.
	 * @param z The z component of the position.
	**/
	void moveto(float x,float y,float z);
};

/**
 * Represents the player.
**/
struct Player{
	/**
	 * The x and y position components of the player's position.
	**/
	float x,y,z;

	/**
	 * The wrapper around the camera.
	**/
	PlayerCamera camera;

	/**
	 * Initialization for time-sensitive data initialization (ex: camera)
	**/
	void init();
};

#endif
