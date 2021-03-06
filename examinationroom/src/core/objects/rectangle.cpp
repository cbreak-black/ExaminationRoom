/*
 *  rectangle.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 21.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "rectangle.h"

#include "surfaces/abstracttexture.h"

#include "parameter/parameterrectangle.h"

#include <qgl.h>

#include "glerrortool.h"

#include "luabridge.hpp"
#include "luahelper.h"

namespace Examination
{
	using namespace Tool;
	
Rectangle::Rectangle()
{
	directionA_.x = 1;
	directionB_.y = 1;
	
	setTexCoords(0,0, 0,1, 1,0, 1,1);
	setSubdivision(2);
	setName("rectangle");
}

ObjectPtr Rectangle::clone() const
{
	return ObjectPtr(new Rectangle(*this));
}

// Drawing
void Rectangle::drawRect(Tool::Point o, Tool::Vector v1, Tool::Vector v2) const
{
	// Subdivision
	unsigned int m, n;
	if (subdivision() > 0)
	{
		float l1 = v1.norm();
		float l2 = v2.norm();
		if (l1 < l2)
		{
			m = subdivision();
			n = l2 / l1 * m;
		}
		else
		{
			n = subdivision();
			m = l1 / l2 * n;
		}
	}
	else
	{
		m = 1;
		n = 1;
	}
	// Find sub-rect vector
	Tool::Vector subV1 = v1/(float)m;
	Tool::Vector subV2 = v2/(float)n;
	// Find normal of the rectangle
	glNormal3fv(cross(v1, v2).vec);
	// Find texture sub vectors
	Tool::Vec2f texV1 = texA();
	Tool::Vec2f texV2 = texB();
	Tool::Vec2f texV3 = texC();
	Tool::Vec2f texV4 = texD();
	// Finally, draw it
	Tool::Point p0 = o; // v2 cursor
	Tool::Point p1; // quad 1 point
	Tool::Point p2; // quad 2 point
	Tool::Vec2f tex; // Texture coordinate
	// Big slices in v2 direction
	for (unsigned int j = 0; j < n; j++)
	{
		// Points
		p1 = p0;
		p0 += subV2;
		p2 = p0;
		// Texture coordinates
		float f1 = 0;
		float f2 = ((float)j)/n;
		float f3 = ((float)(j+1))/n;
		// Small strips in v1 direction
		glBegin(GL_QUAD_STRIP);
		tex = (texV1*(1-f1) + texV3*(f1))*(1-f2) + (texV2*(1-f1) + texV4*(f1))*(f2); // Bilinear interpolation
		glTexCoord2fv(tex.vec);
		glVertex3fv(p1.vec);
		tex = (texV1*(1-f1) + texV3*(f1))*(1-f3) + (texV2*(1-f1) + texV4*(f1))*(f3); // Bilinear interpolation
		glTexCoord2fv(tex.vec);
		glVertex3fv(p2.vec);
		for (unsigned int i = 0; i < m; i++)
		{
			p1 += subV1;
			p2 += subV1;
			f1 = ((float)(i+1))/m;
			tex = (texV1*(1-f1) + texV3*(f1))*(1-f2) + (texV2*(1-f1) + texV4*(f1))*(f2); // Bilinear interpolation
			glTexCoord2fv(tex.vec);
			glVertex3fv(p1.vec);
			tex = (texV1*(1-f1) + texV3*(f1))*(1-f3) + (texV2*(1-f1) + texV4*(f1))*(f3); // Bilinear interpolation
			glTexCoord2fv(tex.vec);
			glVertex3fv(p2.vec);
		}
		glEnd();
	}
}

void Rectangle::draw(GLWidget * dest) const
{
	if (shown())
	{
		GlErrorTool::getErrors("Rectangle::draw:1", name());
		// Set the texture binding
		if (texture())
		{
			texture()->glBindTex(dest);
		}
		// Set wireframe mode
		if (wireframe())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glColor4fv(color().vec);
		drawRect(position(), dirA(), dirB());
		// Reset wireframe state
		if (wireframe())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		// Reset texture binding
		if (texture())
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		GlErrorTool::getErrors("Rectangle::draw:2", name());
	}
}

// Accessors
void Rectangle::setTexCoords(float llx, float lly, float ulx, float uly,
							 float lrx, float lry, float urx, float ury)
{
	objectWillChange();
	texA_ = Vec2f(llx, lly);
	texB_ = Vec2f(ulx, uly);
	texC_ = Vec2f(lrx, lry);
	texD_ = Vec2f(urx, ury);
	objectDidChange();
}

Tool::Vec2f Rectangle::texA() const
{
	return texA_;
}

Tool::Vec2f Rectangle::texB() const
{
	return texB_;
}

Tool::Vec2f Rectangle::texC() const
{
	return texC_;
}

Tool::Vec2f Rectangle::texD() const
{
	return texD_;
}

void Rectangle::setTexA(const Tool::Vec2f & v)
{
	objectWillChange();
	texA_ = v;
	objectDidChange();
}

void Rectangle::setTexB(const Tool::Vec2f & v)
{
	objectWillChange();
	texB_ = v;
	objectDidChange();
}

void Rectangle::setTexC(const Tool::Vec2f & v)
{
	objectWillChange();
	texC_ = v;
	objectDidChange();
}

void Rectangle::setTexD(const Tool::Vec2f & v)
{
	objectWillChange();
	texD_ = v;
	objectDidChange();
}


Tool::Vector Rectangle::dirA() const
{
	return directionA_;
}

Tool::Vector Rectangle::dirB() const
{
	return directionB_;
}

void Rectangle::setDirA(Tool::Vector v)
{
	objectWillChange();
	directionA_ = v;
	objectDidChange();
}

void Rectangle::setDirB(Tool::Vector v)
{
	objectWillChange();
	directionB_ = v;
	objectDidChange();
}

int Rectangle::subdivision() const
{
	return subdivision_;
}

void Rectangle::setSubdivision(int subdivision)
{
	objectWillChange();
	subdivision_ = subdivision < 0 ? 0 : subdivision;
	objectDidChange();
}

// Serialisation
const char * Rectangle::className_ = "Rectangle";

std::string Rectangle::className() const
{
	return Rectangle::className_;
}

std::string Rectangle::toLua(std::ostream & outStream) const
{
	Object::toLua(outStream);
	outStream << name() << ":" << "setSubdivision(" << subdivision() << ");\n";
	outStream << name() << ":" << "setTexCoords("
		<< texA().x << ", " << texA().y << ", "
		<< texB().x << ", " << texB().y << ", "
		<< texC().x << ", " << texC().y << ", "
		<< texD().x << ", " << texD().y << ");\n";
	outStream << name() << ":" << "setDirA({" << dirA().x << ", " << dirA().y << ", " << dirA().z << "});\n";
	outStream << name() << ":" << "setDirB({" << dirB().x << ", " << dirB().y << ", " << dirB().z << "});\n";
	return name();
}

// LUA
void Rectangle::registerLuaApi(luabridge::module * m)
{
	m->subclass<Rectangle,Object>(Rectangle::className_)
	.constructor<void (*)()>()
	.method("dirA", &Rectangle::dirA)
	.method("dirB", &Rectangle::dirB)
	.method("setDirA", &Rectangle::setDirA)
	.method("setDirB", &Rectangle::setDirB)
	.method("subdivision", &Rectangle::subdivision)
	.method("setSubdivision", &Rectangle::setSubdivision)
	.method("setTexCoords", &Rectangle::setTexCoords);
}

std::tr1::shared_ptr<ParameterObject> Rectangle::createDialog()
{
	return std::tr1::shared_ptr<ParameterObject>(new ParameterRectangle(sharedPtr()));
}

}
