/*
 *  lightnode.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef LIGHT_NODE_H
#define LIGHT_NODE_H

#include "container.h"

namespace Examination
{

/**
This class represents a lightable group of objects.
The contained objects are all lit with the defined light.
The number of lights that can be active at the same time is dependent on OpenGL,
and should always be at least 8.
 \author Gerhard Roethlin
 \ingroup Objects
*/
class LightNode : public Container
{
	/**
	All available lights, corresponding in meaning to GL_LIGHT0
	to GL_LIGHT7 (but not in value).
	*/
	typedef enum {
		Light0,
		Light1,
		Light2,
		Light3,
		Light4,
		Light5,
		Light6,
		Light7,
		Invalid
	} LightID;

public: // Construction
	/**
	Creates a light node with default parameters.
	The default name is "Light", the default ambient color is (0,0,0,1).
	 \warning Don't create instances of this class directly, use Object::Create()
	 \see Object::Create()
	*/
	LightNode();

public:
	/**
	Returns the color of the ambient light.
	 \see Object::color() for diffuse color
	 \return the color of the ambient light
	*/
	Tool::Color4 ambient() const;

	/**
	Sets the color of the ambient light.
	 \see Object::setColor() for diffuse color
	 \param color	the new color of the ambient light
	*/
	void setAmbient(Tool::Color4 color);

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

protected: // Parameter Dialog
	virtual std::tr1::shared_ptr<ParameterObject> createDialog();

private: // Light properties
	Tool::Color4 ambient_;

protected:
	static LightID reserveLight();
	static void freeLight(LightID lightID);
	static int toGlLight(LightID lightID);
	static LightID fromGlLight(int glLight);
private: // Static OpenGL light management
	static bool lightActive [8];
};

}

#endif
