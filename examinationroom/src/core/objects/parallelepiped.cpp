/*
 *  rectangle.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 21.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parallelepiped.h"

#include "surfaces/abstracttexture.h"

#include "parameter/parameterparallelepiped.h"

#include <qgl.h>

#include "luabridge.hpp"
#include "luahelper.h"

namespace Examination
{
	using namespace Tool;
	
Parallelepiped::Parallelepiped()
{
	directionC_.z = 1;
	setName("parallelepiped");
}

// Drawing
void Parallelepiped::draw(GLWidget * dest) const
{
	if (shown())
	{
		if (texture())
		{
			texture()->glBindTex(dest);
		}
		glColor4fv(color().vec);
		// Set wireframe mode
		if (wireframe())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glColor4fv(color().vec);
		Tool::Point p1 = position();
		Tool::Point p2 = p1 + dirA() + dirB() + dirC();
		drawRect(p1, dirB(), dirA());
		drawRect(p1, dirA(), dirC());
		drawRect(p1, dirC(), dirB());
		drawRect(p2, -dirA(), -dirB());
		drawRect(p2, -dirC(), -dirA());
		drawRect(p2, -dirB(), -dirC());
		// Reset wireframe state
		if (wireframe())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (texture())
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

Tool::Vector Parallelepiped::dirC() const
{
	return directionC_;
}

void Parallelepiped::setDirC(Tool::Vector v)
{
	objectWillChange();
	directionC_ = v;
	objectDidChange();
}

// Serialisation
const char * Parallelepiped::className_ = "Parallelepiped";

std::string Parallelepiped::className() const
{
	return Parallelepiped::className_;
}

std::string Parallelepiped::toLua(std::ostream & outStream) const
{
	Rectangle::toLua(outStream);
	outStream << name() << ":" << "setDirC({" << dirC().x << ", " << dirC().y << ", " << dirC().z << "});\n";
	return name();
}

// LUA
void Parallelepiped::registerLuaApi(luabridge::module * m)
{
	m->subclass<Parallelepiped,Rectangle>(Parallelepiped::className_)
	.constructor<void (*)()>()
	.method("dirC", &Parallelepiped::dirC)
	.method("setDirC", &Parallelepiped::setDirC);
}

std::tr1::shared_ptr<ParameterObject> Parallelepiped::createDialog()
{
	return std::tr1::shared_ptr<ParameterObject>(new ParameterParallelepiped(sharedPtr()));
}

}
