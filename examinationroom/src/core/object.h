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

#ifndef OBJECT_H
#define OBJECT_H

#include "vec.h"
#include "drawing.h"

namespace Examination
{

/**
A generic object, which can be placed in a scene.

\author Gerhard Roethlin
*/
class Object
{

public:
	/**
	Creator of Objects.
	*/
    Object();
    
	/**
	Creator of Objects.
	\param x	X Coordinate of the object
	\param y	Y Coordinate of the object
	\param z	Z Coordinate of the object
	*/
    Object(float x, float y, float z);

	/**
	Creator of Objects.
	 \param o	Origin  of the object
	 */
    Object(tool::Point o);
	
	/**
	Destructor of Objects
	*/
	virtual ~Object();
	
public: // Drawing
	virtual void draw(Side s) = 0;

private:
	tool::Point origin_;
};

}

#endif
