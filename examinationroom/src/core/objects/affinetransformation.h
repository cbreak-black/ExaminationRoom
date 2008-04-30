/*
 *  affinetransformation.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef AFFINE_TRANSFORMATION_H
#define AFFINE_TRANSFORMATION_H

#include "container.h"

#include "vec.h"

namespace Examination
{

/**
This class represents a transformable group of objects.
It internaly represents the transformations as 4x4 matrix with OpenGL ordering.
 \author Gerhard Roethlin
 \ingroup Objects
*/
class AffineTransformation : public Container
{
public: // Construction
	/**
	Creates an affine transformation object with the identity transformation.
	The default name is "Transformation".
	*/
	AffineTransformation();

public:
	/**
	Loads the identity matrix into the transformation matrix.
	*/
	void loadIdentity();

	/**
	Multiplies the given matrix with the stored matrix to get the stored matrix.
	 \param m	A pointer to a 16 element array in OpenGL ordering.
	*/
	void multMatrix(double * m);

	/**
	Translates the object by the given vector.
	This is cumulative with previous transformations.
	 \param t	Translation vector
	*/
	void translate(Tool::Vector t);

	/**
	Rotates the object by the given angle around the given vector.
	This is cumulative with previous transformations.
	 \param axis	Rotation axis
	 \param angle	Rotation angle
	*/
	void rotate(Tool::Vector axis, double angle);

	/**
	Scales the object by a given factor in x, y and z direction.
	This is cumulative with previous transformations.
	 \param scale	Scale factor vector
	*/
	void scale(Tool::Vec3f scale);

public:
	virtual void draw(GLWidget * dest) const;

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
	double trans_[16];
};

}

#endif
