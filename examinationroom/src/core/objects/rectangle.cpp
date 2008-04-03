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
#include "glwidget.h"
#include "scene.h"
#include "camera.h"

#include "screenproject.h"
#include "platform_math.h"

namespace Examination
{
	using namespace Tool;
	
Rectangle::Rectangle()
{
	directionA_.x = 1;
	directionB_.y = 1;
	
	setTexCoords(0,0, 0,1, 1,0, 1,1);
}

Rectangle::~Rectangle()
{
}

// Drawing
void Rectangle::draw(GLWidget * dest) const
{
	Point v1 = position();
	Point v2 = position() + dirB();
	Point v3 = position() + dirA();
	Point v4 = position() + dirA() + dirB();

	if (texture())
	{
		texture()->glBindTex(dest);
	}

	glColor4fv(color().vec);
	
	if (wireframe())
	{
		glBegin(GL_LINE_LOOP);
		glVertex3fv(v1.vec);
		glVertex3fv(v2.vec);
		glVertex3fv(v4.vec);
		glVertex3fv(v3.vec);
		glEnd();
	}
	else
	{
		glBegin(GL_TRIANGLE_STRIP);
		Vector n = cross(dirA(), dirB());
		//n.normalize(); // OpenGL will normalize
		glNormal3fv(n.vec);
		glTexCoord2f(texA_.x, texA_.y); glVertex3fv(v1.vec);
		glTexCoord2f(texB_.x, texB_.y); glVertex3fv(v2.vec);
		glTexCoord2f(texC_.x, texC_.y); glVertex3fv(v3.vec);
		glTexCoord2f(texD_.x, texD_.y); glVertex3fv(v4.vec);
		glEnd();
	}

	if (texture())
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

// Accessors
void Rectangle::setTexCoords(float llx, float lly, float ulx, float uly,
							 float lrx, float lry, float urx, float ury)
{
	texA_ = Point(llx, lly, 0);
	texB_ = Point(ulx, uly, 0);
	texC_ = Point(lrx, lry, 0);
	texD_ = Point(urx, ury, 0);
}

Tool::Point Rectangle::texA() const
{
	return texA_;
}

Tool::Point Rectangle::texB() const
{
	return texB_;
}

Tool::Point Rectangle::texC() const
{
	return texC_;
}

Tool::Point Rectangle::texD() const
{
	return texD_;
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
	directionA_ = v;
}

void Rectangle::setDirB(Tool::Vector v)
{
	directionB_ = v;
}
	
}
