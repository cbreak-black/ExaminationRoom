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

public: // Textures
	/**
	Sets the texture coordinates of this rectangle. Default is
	(0,0) (0,1) (1,0) (1,1)
	 \param llx	lower left corner, x coordinate
	 \param lly	lower left corner, y coordinate
	 \param ulx	lower left corner, x coordinate
	 \param uly	lower left corner, y coordinate
	 \param lrx	lower left corner, x coordinate
	 \param lry	lower left corner, y coordinate
	 \param urx	lower left corner, x coordinate
	 \param ury	lower left corner, y coordinate
	*/
	void setTexCoords(float llx, float lly, float ulx, float uly,
					  float lrx, float lry, float urx, float ury);

private:
	Tool::Vector directionC_;
};
	
}

#endif
