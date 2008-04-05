/*
 *  atmosphere.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 02.04.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include "object.h"
#include "container.h"

namespace Examination
{

/**
This class represents a group of objects that are influenced by atmospheric effects
such as fog.
*/
class Atmosphere : public Container
{
public:
	/**
	The fog mode of this atmosphere.
	Exp: Exponential fog, $f=e^{-(density.z)}$
	Exp2: Bi-Exponential fog, $f=e^{-(density.z)^2}$
	Linear: Linear fog, $f=\frac{end-z}{end-start}$
	*/
	typedef enum { Exp, Exp2, Linear } FogMode;

public: // Construction
	Atmosphere();

public:
	/**
	Gets the fog mode of the object. This is a one-to-one mapping to OpenGL's capabilities.
	Default value is "Exp".
	 \see FogMode
	 \return The currently set FogMode
	*/
	FogMode mode() const;

	/**
	Sets the fog mode of the object. This is a one-to-one mapping to OpenGL's capabilities.
	 \param fogMode the desired fog mode.
	*/
	void setMode(FogMode fogMode);

	/**
	Returns the density used in the exponential fog equations. Default value is 1;
	 \return the density used in the exponential fog equation
	*/
	float density() const;

	/**
	Sets the density used in the exponential fog equations.
	 \param density	the density used in the exponential fog equation. Only positive values allowed
	*/
	void setDensity(float density);

	/**
	Returns the start distance used in the linear fog equation. Default is "0".
	 \return the start distance used in the linear fog equation
	*/
	float start() const;

	/**
	Sets the start distance used in the linear fog equation.
	 \param start	Start distance for linear fog equation
	*/
	void setStart(float start);

	/**
	Returns the end distance used in the linear fog equation. Default is "100".
	 \return the end distance used in the linear fog equation
	*/
	float end() const;

	/**
	Sets the end distance used in the linera for equation
	 \param end	End distance in the linera fog equation
	*/
	void setEnd(float end);

public:
	virtual void draw(GLWidget * dest) const;

private:
	FogMode fogMode_;
	float density_;
	float start_;
	float end_;
};

}

#endif
