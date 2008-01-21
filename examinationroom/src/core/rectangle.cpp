/*
 *  rectangle.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 21.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "rectangle.h"

namespace Examination
{
	using namespace Tool;
	
Rectangle::Rectangle()
{
	directionA_.x = 1;
	directionB_.y = 1;
}

void Rectangle::draw(Side s)
{
	if (s == right)
	{
	}
	else
	{
	}
	
	Point v1 = position() + dirA() + dirB();
	Point v2 = position() + dirA() - dirB();
	Point v3 = position() - dirA() + dirB();
	Point v4 = position() - dirA() - dirB();
	
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(v1.vec);
	glVertex3fv(v2.vec);
	glVertex3fv(v3.vec);
	glVertex3fv(v4.vec);
	glEnd();
}
	
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
	
	
	
}
