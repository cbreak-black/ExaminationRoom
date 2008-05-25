/*
 *  cameranode.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 03.26.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "cameranode.h"

#include "camera.h"

#include <qgl.h>

#include "luabridge.hpp"
#include "luahelper.h"

using namespace std::tr1;

namespace Examination
{

CameraNode::CameraNode()
{
	cam_ = shared_ptr<Camera>(new Camera());
	setDrawPriority(128);
	setName("cameranode");
}

CameraNode::CameraNode(const CameraNode & c)
	: Container(c)
{
	cam_ = shared_ptr<Camera>(new Camera(*(c.camera())));
}

ObjectPtr CameraNode::clone() const
{
	ContainerPtr c(new CameraNode(*this));
	c->clone(this);
	return c;
}

std::tr1::shared_ptr<Camera> CameraNode::camera() const
{
	return cam_;
}

void CameraNode::setCamera(std::tr1::shared_ptr<Camera> camera)
{
	cam_ = camera;
}

// Serialisation
const char * CameraNode::className_ = "CameraNode";

std::string CameraNode::className() const
{
	return CameraNode::className_;
}

/**
\todo	Add support for camera export
*/
std::string CameraNode::toLua(std::ostream & outStream) const
{
	Container::toLua(outStream);
	// Localy scoped variable c points to camera
	std::string c = "cam";
	outStream << "do -- cam\nlocal " << c << " = " << name() << ":camera();\n";
	camera()->toLua(outStream, c);
	outStream << "end --cam\n";
	return name();
}

// LUA
void CameraNode::registerLuaApi(luabridge::module * m)
{
	m->subclass<CameraNode,Container>(CameraNode::className_)
	.constructor<void (*)()>()
	.method("camera", &CameraNode::camera)
	.method("setCamera", &CameraNode::setCamera);
}

// Drawing
void CameraNode::draw(GLWidget * dest) const
{
	if (shown())
	{
		// If not enabled, just draw children and return
		if (!enabled())
		{
			Container::draw(dest);
		}
		else
		{
			// Push the matrix stack, so that the previous camera can be restored
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			// Remove old data from depth buffer
			glClear(GL_DEPTH_BUFFER_BIT);
			// Set up camera
			camera()->loadMatrix(dest);
			// Draw the contents of this node
			Container::draw(dest);
			// And restore the matrix stack
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}
}

}
