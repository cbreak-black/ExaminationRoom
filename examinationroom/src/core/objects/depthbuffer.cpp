/*
 *  depthbuffer.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.08.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "depthbuffer.h"

#include "parameter/parameterdepthbuffer.h"

#include <qgl.h>

#include "luabridge.hpp"
#include "luahelper.h"

namespace Examination
{

DepthBuffer::DepthBuffer()
{
	setDepthBufferState(true);
	setDrawPriority(128);
	setName("depthbuffer");
}

ObjectPtr DepthBuffer::clone() const
{
	ContainerPtr c(new DepthBuffer(*this));
	c->clone(this);
	return c;
}

// Drawing
void DepthBuffer::draw(GLWidget * dest) const
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
			//Container::draw(dest);
			const Container::ObjectList & ol = Container::objects();
			Container::ObjectList::const_iterator it = ol.begin();
			Container::ObjectList::const_iterator end = ol.end();
			for (; it != end; it++)
			{
				// Clear depth buffer if it's enabled
				if (depthBufferState())
					glClear(GL_DEPTH_BUFFER_BIT);
				(*it)->draw(dest);
			}
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
}

bool DepthBuffer::depthBufferState() const
{
	return depthBufferState_;
}

void DepthBuffer::setDepthBufferState(bool enabled)
{
	objectWillChange();
	depthBufferState_ = enabled;
	objectDidChange();
}

// Serialisation
const char * DepthBuffer::className_ = "DepthBuffer";

std::string DepthBuffer::className() const
{
	return DepthBuffer::className_;
}

std::string DepthBuffer::toLua(std::ostream & outStream) const
{
	Container::toLua(outStream);
	outStream << name() << ":" << "setDepthBufferState(" << (depthBufferState() ? "true" : "false") << ");\n";
	return name();
}

// LUA
void DepthBuffer::registerLuaApi(luabridge::module * m)
{
	m->subclass<DepthBuffer,Container>("DepthBuffer")
	.constructor<void (*)()>()
	.method("depthBufferState", &DepthBuffer::depthBufferState)
	.method("setDepthBufferState", &DepthBuffer::setDepthBufferState);
}

std::tr1::shared_ptr<ParameterObject> DepthBuffer::createDialog()
{
	return std::tr1::shared_ptr<ParameterObject>(new ParameterDepthBuffer(sharedPtr()));
}

}
