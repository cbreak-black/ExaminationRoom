/*
 *  rectangle.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 21.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <memory>

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
	/**
	Sets the texture for this object.
	 \param t	Texture for left camera
	*/
	void setTexture(std::tr1::shared_ptr<AbstractTexture> t);

private:
	Tool::Vector directionA_;
	Tool::Vector directionB_;
	
	Tool::Point texA_;
	Tool::Point texB_;
	Tool::Point texC_;
	Tool::Point texD_;

	std::tr1::shared_ptr<AbstractTexture> tex_;
};
	
}

#endif
