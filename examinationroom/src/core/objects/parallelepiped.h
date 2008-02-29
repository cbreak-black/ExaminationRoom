/*
 *  parallelepiped.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 27.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PARALLELEPIPED_H
#define PARALLELEPIPED_H

#include <memory>

#include "rectangle.h"

namespace Examination
{
	class AbstractTexture;
	
class Parallelepiped : public Rectangle
{
public: // Construction
	Parallelepiped();

public:
	virtual void draw(GLWidget * dest);
	
public:
	/**
	Returns the third direction of this rectangle.
	 \return the third direction of this rectangle.
	 */
	Tool::Vector dirC();

	/**
	Sets the third direction of the rectangle.
	 \param v	the new direction
	*/
	void setDirC(Tool::Vector v);

private:
	Tool::Vector directionC_;
};
	
}

#endif
