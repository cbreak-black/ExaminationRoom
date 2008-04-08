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

#include "object.h"
#include "container.h"

namespace Examination
{

/**
This class represents a lightable group of objects.
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
