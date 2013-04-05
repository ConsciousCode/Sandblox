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
	along with Sandblox.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SANDBLOX_VERSION_H
#define SANDBLOX_VERSION_H

#include <Python.h>
#include <structmember.h>

#include <string>

/**
 * Represents a version, used in the mod and sandblox modules.
**/
struct Version{
	PyObject_HEAD

	unsigned char major;
	unsigned char minor;
	unsigned char build;
	unsigned char revision;
};

int version_init(Version*,PyObject*,PyObject*);
PyObject* version_repr(Version*);
PyObject* version_str(Version*);

/**
 * Convert a version string to a packed integer.
 *
 * @param s The version string (format: byte[.byte[.byte[.byte]]], ex: "1.0.4.37")
 *
 * @return A packed integer representing the version.
**/
unsigned s2version(const std::string& s);

/**
 * Make a new Version instance.
 *
 * @param s - The properly formatted string containing the version.
**/
PyObject* Version_New(const char* s);

/**
 * Make a new Version instance
 *
 * @param v - A packed integer containing the version.
**/
PyObject* Version_New(unsigned v);

extern PyMemberDef Version_members[];

extern PyTypeObject VersionType;

#endif
