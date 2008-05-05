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

#include "object.h"

namespace Examination
{
	class AbstractTexture;

/**
The rectangle class represents a 2D parallelogram object. It is defined by it's
origin, and two vectors.
 \ingroup Objects
*/
class Rectangle : public Object
{
public: // Construction
	/**
	Creates a default rectangle with side length 1, parallel to x and y axis.
	The default texture coordinates map the lower left corner of the texture to
	the origin, the lower right to the point at dirA, and the upper left to dirB.
	By default, it is subdivided in two segments.
	The default name is "Rectangle".
	 \warning Don't create instances of this class directly, use Object::Create()
	 \see Object::Create()
	*/
	Rectangle();

public:
	virtual void draw(GLWidget * dest) const;

protected:
	/**
	Draws the parallelogram defined by a point and two vectors.
	 \param o	Origin point
	 \param v1	Vector 1
	 \param	v2	Vector 2
	*/
	void drawRect(Tool::Point o, Tool::Vector v1, Tool::Vector v2) const;

public:
	/**
	Returns the first direction of this rectangle.
	 \return the first direction of this rectangle.
	*/
	Tool::Vector dirA() const;

	/**
	Returns the second direction of this rectangle.
	 \return the second direction of this rectangle.
	 */
	Tool::Vector dirB() const;

	/**
	Sets the first direction of the rectangle.
	 \param v	the new direction
	*/	
	void setDirA(Tool::Vector v);
	
	/**
	Sets the first direction of the rectangle.
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
	Tool::Vec2f texA() const;
	Tool::Vec2f texB() const;
	Tool::Vec2f texC() const;
	Tool::Vec2f texD() const;

public: // Subdiv
	/**
	Returns the number of subdivisions of the rectangle.
	0 means no subdivision.
	n where n is 1 or more means that the shorter side is subdivided n times.
	The longer side is subdivided so that the subdivisions are as little skewed as possible.
	 \return the number of subdivisions of the rectangle along the short side
	*/
	unsigned int subdivision() const;

	/**
	Sets the number of subdivisions of the rectangle.
	 \see subdivision()
	 \param subdivision	Number of desired subdivisions along the short side
	*/
	void setSubdivision(unsigned int subdivision);

public: // Serialisation
	/**
	Returns the name of the class of this object. This can be used in LUA
	object creation.
	 \return The name of this object's class as c++ string
	*/
	virtual std::string className() const;

	/**
	Writes the LUA commands to set parameters of this object to the output stream.
	 \param outStream	A stream that accepts writing
	*/
	virtual std::string toLua(std::ostream & outStream) const;

private:
	Tool::Vector directionA_;
	Tool::Vector directionB_;
	
	Tool::Vec2f texA_;
	Tool::Vec2f texB_;
	Tool::Vec2f texC_;
	Tool::Vec2f texD_;

	unsigned int subdivision_;
};
	
}

#endif
