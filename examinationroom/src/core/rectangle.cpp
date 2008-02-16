/*
 *  rectangle.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 21.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "rectangle.h"

#include "abstracttexture.h"
#include "glwidget.h"

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
void Rectangle::draw(GLWidget * dest)
{
	Point v1 = position() - dirA() - dirB();
	Point v2 = position() - dirA() + dirB();
	Point v3 = position() + dirA() - dirB();
	Point v4 = position() + dirA() + dirB();

	if (texture())
	{
		if (autoResize())
		{
			ScreenProject sp;
			sp.calculateMVP();
			int w, h, cw, ch;
			Point v1p = sp.transformToScreenSpace(v1);
			Point v2p = sp.transformToScreenSpace(v2);
			Point v3p = sp.transformToScreenSpace(v3);
			Point v4p = sp.transformToScreenSpace(v4);
			h = abs(v2p.y - v1p.y);
			w = abs(v3p.x - v1p.x);
			cw = texture()->width();
			ch = texture()->height();
			if (abs(cw-w) + abs(ch-h) > 2)
			{
				texture()->resizeTo(w,h);
			}
		}
		texture()->glBindTex(dest);
	}
	
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(texA_.x, texA_.y); glVertex3fv(v1.vec);
	glTexCoord2f(texB_.x, texB_.y); glVertex3fv(v2.vec);
	glTexCoord2f(texC_.x, texC_.y); glVertex3fv(v3.vec);
	glTexCoord2f(texD_.x, texD_.y); glVertex3fv(v4.vec);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);
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
