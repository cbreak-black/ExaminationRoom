/*
 *  rectangle.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 12.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "pixelplane.h"

#include "abstracttexture.h"
#include "glwidget.h"

#include "screenproject.h"
#include "platform_math.h"

namespace Examination
{
	using namespace Tool;
	
Pixelplane::Pixelplane()
{
	width_ = 1;
	height_ = 1;
}

Pixelplane::~Pixelplane()
{
}

// Drawing
void Pixelplane::draw(GLWidget * dest)
{
	if (texture())
	{
		if (autoResize())
		{
			ScreenProject sp;
			sp.calculateMVP();
			int w, h, cw, ch;
			Point p = position();
			Point q = Point(p.x+width_, p.y, p.z);
			Point r = Point(p.x, p.y+height_, p.z);
			Point pp = sp.transformToScreenSpace(p);
			Point qp = sp.transformToScreenSpace(q);
			Point rp = sp.transformToScreenSpace(r);
			h = abs(pp.y - rp.y);
			w = abs(pp.x - qp.x);
			cw = texture()->width();
			ch = texture()->height();
			if (abs(cw-w) + abs(ch-h) > 2)
			{
				texture()->resizeTo(w,h);
			}
		}
		Point p = position();
		glRasterPos3f(p.x, p.y, p.z);
		texture()->draw(dest);
	}
}

void Pixelplane::setSize(float w, float h)
{
	width_ = w;
	height_ = h;
}
	
}
