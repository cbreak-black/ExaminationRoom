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

using namespace std::tr1;

namespace Examination
{

CameraNode::CameraNode()
{
	cam_ = shared_ptr<Camera>(new Camera());
	setDrawPriority(128);
	setName("Camera");
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
std::string CameraNode::className() const
{
	return "CameraNode";
}

/**
\todo	Add support for camera export
*/
std::string CameraNode::toLua(std::ostream & outStream) const
{
	Container::toLua(outStream);
	outStream << "-- Camera Write not supported yet\n";
	return name();
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
