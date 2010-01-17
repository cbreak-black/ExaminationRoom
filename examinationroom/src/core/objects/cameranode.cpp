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

#include "parameter/parametercameranode.h"

#include <qgl.h>

#include "glerrortool.h"

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

std::tr1::shared_ptr<ParameterObject> CameraNode::createDialog()
{
	return std::tr1::shared_ptr<ParameterObject>(new ParameterCameraNode(sharedPtr()));
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
			// Remove old data from depth buffer
			glClear(GL_DEPTH_BUFFER_BIT);
			// Set up camera
			camera()->loadMatrix(dest);
			// Draw the contents of this node
			GlErrorTool::getErrors("CameraNode::draw:1", name());
			Container::draw(dest);
			// And restore the matrix stack
			camera()->unloadMatrix();
			GlErrorTool::getErrors("CameraNode::draw:2", name());
		}
	}
}

}
