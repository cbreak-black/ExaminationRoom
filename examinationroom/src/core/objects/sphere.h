/*
 *  sphere.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 02.04.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

namespace Examination
{

/**
This class represents a sphere.
The drawing subdivision is parametrized.
 \ingroup Objects
*/
class Sphere : public Object
{
public: // Construction
	/**
	Creates a scene with origin at the scene origin, and radius 1.
	The default subdivision uses 24 slices and 12 stacks.
	The default name is "Sphere".
	 \warning Don't create instances of this class directly, use Object::Create()
	 \see Object::Create()
	*/
	Sphere();

public:
	virtual void draw(GLWidget * dest) const;

public:
	/**
	Returns the current radius of the sphere.
	 \return the current radius of the sphere
	*/
	float radius() const;

	/**
	Sets the radius of the sphere.
	 \param radius	the new radius
	*/
	void setRadius(float radius);

	/**
	Returns the number of slices of this sphere.
	Think of longitude division (East-West).
	 \return the number of slices
	*/
	int slices() const;

	/**
	Sets the number of slices of this sphere.
	 \param slices	The number of slices the sphere should have
	*/
	void setSlices(int slices);

	/**
	Returns the number of stacks of this sphere.
	Think of latitude division (North-South).
	 \return the number of stacks of the sphere
	*/
	int stacks() const;

	/**
	Sets the number of stacks of this sphere.
	 \param stacks	The number of stacks the sphere should have
	*/
	void setStacks(int stacks);

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
	float radius_;
	int slices_;
	int stacks_;
};
	
}

#endif
