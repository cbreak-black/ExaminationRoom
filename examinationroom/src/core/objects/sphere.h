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
	
class Sphere : public Object
{
public: // Construction
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

private:
	float radius_;
	int slices_;
	int stacks_;
};
	
}

#endif
