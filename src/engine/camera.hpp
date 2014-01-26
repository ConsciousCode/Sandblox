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

/**
 * @file engine/camera.hpp
 *
 * Defines a generalized container for a camera.
**/

#pragma once

#ifndef SANDBLOX_ENGINE_CAMERA
#define SANDBLOX_ENGINE_CAMERA

/**
 * An abstract class to encapsulate the 3D engine's camera.
**/
class Camera{
	public:
		virtual ~Camera(){}

		/**
		 * Move the camera to the given 3D point.
		**/
		virtual void move(float x,float y,float z)=0;

		/**
		 * Translate the camera by the given amount in each direction relative
		 * to the direction the camera is pointing (the local coordinate system
		 * facing down the positive x axis).
		**/
		virtual void translate(float x,float y,float z)=0;

		/**
		 * Rotate the camera by the given yaw (rotation about z), pitch (rotation
		 * about y), and roll (rotation about x). Values are percents of rotation, 0-1.
		**/
		virtual void rotate(float z,float y,float x=0.0f)=0;
};

#endif
