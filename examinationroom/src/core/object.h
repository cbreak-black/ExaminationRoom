/*
 *  object.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "vec.h"

namespace Examination
{
	class GLWidget;

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
    Object(Tool::Point o);

	/**
	Destructor of Objects
	*/
	virtual ~Object();

public: // Accessors
	/**
	Returns the position of this object.
	 \return the position of this object.
	*/
	Tool::Point position();

	/**
	Sets the position of the object.
	 \param p	the new position of the object.
	*/
	void setPosition(Tool::Point p);
	
public: // Drawing
	virtual void draw(GLWidget * dest) = 0;

private:
	Tool::Point origin_;
};

}

#endif
