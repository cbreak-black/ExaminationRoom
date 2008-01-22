/*
 *  rectangle.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 21.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "object.h"

namespace Examination
{
	class AbstractTexture;
	
class Rectangle : public Object
{
public: // Construction
	Rectangle();
	~Rectangle();

public:
	virtual void draw(GLWidget * dest);
	
public:
	/**
	Returns the first direction of this rectangle.
	 \return the first direction of this rectangle.
	*/
	Tool::Vector dirA();

	/**
	Returns the second direction of this rectangle.
	 \return the second direction of this rectangle.
	 */
	Tool::Vector dirB();

	/**
	Sets the first direction of the rectangle.
	 \warn	Make sure that both vectors are normal to each other
	 \param v	the new direction
	*/	
	void setDirA(Tool::Vector v);
	
	/**
	Sets the first direction of the rectangle.
	 \warn	Make sure that both vectors are normal to each other
	 \param v	the new direction
	*/
	void setDirB(Tool::Vector v);

public: // Textures
	/**
	Sets the texture for this object.
	 \param t	Texture for left camera
	*/
	void setTexture(AbstractTexture * t);

private:
	Tool::Vector directionA_;
	Tool::Vector directionB_;

	AbstractTexture * tex_;
};
	
}

#endif
