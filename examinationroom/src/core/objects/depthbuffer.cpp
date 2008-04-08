/*
 *  depthbuffer.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.08.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "depthbuffer.h"

#include <qgl.h>

namespace Examination
{

DepthBuffer::DepthBuffer()
{
	setDepthBufferState(false);
	setDrawPriority(128);
}

// Drawing
void DepthBuffer::draw(GLWidget * dest) const
{
	if (shown())
	{
		// Clear depth buffer state
		glClear(GL_DEPTH_BUFFER_BIT);
		// Store and set the depth buffer state
		bool currentState = glIsEnabled(GL_DEPTH_TEST);
		if (depthBufferState())
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
		// Draw the contents of this node
		Container::draw(dest);
		if (currentState)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
}

bool DepthBuffer::depthBufferState() const
{
	return depthBufferState_;
}

void DepthBuffer::setDepthBufferState(bool enabled)
{
	depthBufferState_ = enabled;
}

}
