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
	if (shown())
	{
		// Reserve a light id
		LightID id = reserveLight();
		if (id == Invalid)
		{
			// No light free
			return;
		}
		int glID = toGlLight(id);
		// Store the old enabled state
		GLboolean isEnabled[4];
		glGetBooleanv(GL_NORMALIZE, isEnabled+0);
		glGetBooleanv(GL_LIGHTING, isEnabled+1);
		glGetBooleanv(glID, isEnabled+2);
		glGetBooleanv(GL_COLOR_MATERIAL, isEnabled+3);
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
		// GL_POSITION expects homogenous coordinates (4 floats)
		glLightfv(glID, GL_POSITION, toHomogenous(position()).vec);
		// Draw the contents of this node
		Container::draw(dest);
		// Disable lighting and misc stuff (if it was not already enabled)
		if (!isEnabled[3])
			glDisable(GL_COLOR_MATERIAL);
		if (!isEnabled[2])
			glDisable(glID);
		if (!isEnabled[1])
			glDisable(GL_LIGHTING);
		if (!isEnabled[0])
			glDisable(GL_NORMALIZE);
		// Free the light id that was used.
		freeLight(id);
	}
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
