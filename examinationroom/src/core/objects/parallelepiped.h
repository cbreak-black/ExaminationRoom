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

#include "rectangle.h"

namespace Examination
{
	class AbstractTexture;

/**
The Parallelepiped class is a generalisation of the Rectangle class into 3D.
It is defined by three vectors and an origin.
 \author Gerhard Roethlin
 \ingroup Objects
*/
class Parallelepiped : public Rectangle
{
public: // Construction
	/**
	Creates a default parallelepiped.
	It defaults to the unit cube from (0,0,0) to (1,1,1).
	The default name is "Parallelepiped".
	*/
	Parallelepiped();

public: // Cloning
	/**
	Returns a newly allocated clone of this object.
	 \return a newly created copy of this object
	*/
	virtual ObjectPtr clone() const;

public: // Drawing
	virtual void draw(GLWidget * dest) const;

public:
	/**
	Returns the third direction of this rectangle.
	 \return the third direction of this rectangle.
	 */
	Tool::Vector dirC() const;

	/**
	Sets the third direction of the rectangle.
	 \param v	the new direction
	*/
	void setDirC(Tool::Vector v);

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

public: // LUA API
	static void registerLuaApi(luabridge::module * m);

protected: // Parameter Dialog
	virtual std::tr1::shared_ptr<ParameterObject> createDialog();

private:
	Tool::Vector directionC_;

public: // Meta
	static const char * className_;
};
	
}

#endif
