/*
 *  rectangle.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 21.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "rectangle.h"

#include "texture.h"
#include "glwidget.h"

namespace Examination
{
	using namespace Tool;
	
Rectangle::Rectangle()
{
	directionA_.x = 1;
	directionB_.y = 1;

	texLeft_ = 0;
	texRight_ = 0;
}

Rectangle::~Rectangle()
{
	delete texLeft_;
	delete texRight_;
}

// Drawing
void Rectangle::draw(GLWidget * dest)
{
	if (texRight_ && dest->side() == right)
	{
		glBindTexture(GL_TEXTURE_2D, texRight_->glTexID(dest));
	}
	else if (texLeft_)
	{
		glBindTexture(GL_TEXTURE_2D, texLeft_->glTexID(dest));
	}
	
	Point v1 = position() + dirA() + dirB();
	Point v2 = position() + dirA() - dirB();
	Point v3 = position() - dirA() + dirB();
	Point v4 = position() - dirA() - dirB();
	
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(v1.vec);
	glTexCoord2f(0.0f, 4.0f); glVertex3fv(v2.vec);
	glTexCoord2f(2.0f, 0.0f); glVertex3fv(v3.vec);
	glTexCoord2f(2.0f, 4.0f); glVertex3fv(v4.vec);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Accessors
Tool::Vector Rectangle::dirA()
{
	return directionA_;
}

Tool::Vector Rectangle::dirB()
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

// Textures
void Rectangle::setTextures(Texture * left, Texture * right)
{
	texLeft_ = left;
	texRight_ = right;
}	
	
	
}
