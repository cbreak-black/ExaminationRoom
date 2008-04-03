/*
 *  lightnode.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "lightnode.h"

#include <qgl.h>

namespace Examination
{

LightNode::LightNode()
{
	setAmbient(Tool::Color4(0,0,0,1));
}

Tool::Color4 LightNode::ambient() const
{
	return ambient_;
}

void LightNode::setAmbient(Tool::Color4 color)
{
	ambient_ = color;
}

// Drawing
void LightNode::draw(GLWidget * dest) const
{
	// Reserve a light id
	LightID id = reserveLight();
	if (id == Invalid)
	{
		// No light free
		return;
	}
	int glID = toGlLight(id);
	// Set up lighting
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(glID);
	// Set up color material (for setting material with glColor*())
	glEnable(GL_COLOR_MATERIAL);
	// Set Light parameters
	glLightfv(glID, GL_AMBIENT, ambient().vec);
	glLightfv(glID, GL_DIFFUSE, color().vec);
	//glLightfv(glID, GL_SPECULAR, specularLight);
	glLightfv(glID, GL_POSITION, position().vec);
	// Draw the contents of this node
	Container::draw(dest);
	// Disable lighting and misc stuff
	glDisable(GL_COLOR_MATERIAL);
	glDisable(glID);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	// Free the light id that was used.
	freeLight(id);
}

Container * LightNode::getParent()
{
	return this;
}

Scene * LightNode::getScene()
{
	return scene();
}

void LightNode::setScene(Scene * s)
{
	Object::setScene(s);
	setParentsAndScenes(parent(), scene());
}

void LightNode::setParent(Container * c)
{
	Object::setParent(c);
	setParentsAndScenes(parent(), scene());
}

// Static OpenGL Light management
bool LightNode::lightActive [] = {
	false, false, false, false,
	false, false, false, false
};

LightNode::LightID LightNode::reserveLight()
{
	for (int i = 0; i < 8; i++)
	{
		if (!lightActive[i])
		{
			lightActive[i] = true;
			return (LightID)i;
		}
	}
	return Invalid;
}

void LightNode::freeLight(LightID lightID)
{
	if (lightID >= 0 && lightID <= 7)
	{
		lightActive[lightID] = false;
	}
}

int LightNode::toGlLight(LightID lightID)
{
	if (lightID != Invalid)
	{
		// GL_LIGHT0 is defined as 0x4000
		// GL_LIGHT1 to GL_LIGHT7 are consecutively numbered
		return GL_LIGHT0 + (int)lightID;
	}
	else
	{
		return 0;
	}
}

LightNode::LightID LightNode::fromGlLight(int glLight)
{
	// GL_LIGHT0 is defined as 0x4000
	// GL_LIGHT1 to GL_LIGHT7 are consecutively numbered
	int lightID = glLight - GL_LIGHT0;
	if (lightID >= 0 && lightID <= 7)
	{
		// Valid
		return (LightID)lightID;
	}
	else
	{
		return Invalid;
	}
}


}
