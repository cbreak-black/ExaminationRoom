/*******************************************************************************

Examination Room is a flexible user evaluation tool

Copyright (C) 2008 Gerhard Roethlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************************/

#ifndef SCENE_H
#define SCENE_H

#include <set>

#include "drawing.h"

namespace Examination
{
	
class Object;

/**
This class represents the scene. It contains references to objects.

\author Gerhard Roethlin
*/
class Scene
{

public:
	/**
	Creator of scenes.
	*/
    Scene();

public: // Drawing
	/**
	Draw this object for the side s.
	 \param s	the side to draw for.
	*/
	void drawScene(Side s);
	
public: // Scene management
	/**
	Adds an object to the scene. An object can only be added once.
	 \param object	A pointer to an object
	 \return	true if the object was inserted, false if it already was in the scene.
	*/
	bool addOject(Object * object);
	
	/**
	Removes an object from the scene.
	 \param object	A pointer to an object
	*/
	void removeOject(Object * object);
		
private:
	std::set<Object*> objects_;

};

}

#endif
