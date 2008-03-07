/*
 *  rectangle.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 12.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "pixelplane.h"

#include "surfaces/abstracttexture.h"
#include "glwidget.h"
#include "scene.h"
#include "camera.h"
#include "platform_math.h"

namespace Examination
{
	using namespace Tool;
	
Pixelplane::Pixelplane()
{
	setAutoResize(false);
	setSize(1,1);
}

Pixelplane::~Pixelplane()
{
}

// Drawing
void Pixelplane::draw(GLWidget * dest) const
{
	if (texture())
	{
		if (autoResize())
		{
			resizeToCurrent();
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

void Pixelplane::setAutoResize(bool b)
{
	autoresize_ = b;
}

bool Pixelplane::autoResize() const
{
	return autoresize_;
}

void Pixelplane::resizeToCurrent() const
{
	if (scene() && texture())
	{
		std::tr1::shared_ptr<Camera> cam = scene()->camera();
		int w, h, cw, ch;
		float uss = cam->unitScreenSize(position());
		w = uss*width_;
		h = uss*height_;
		cw = texture()->width();
		ch = texture()->height();
		if (cw != w || ch != h)
		{
			Pixelplane::resizeTo(w,h);
		}
	}
}

void Pixelplane::resizeTo(int w, int h) const
{
	if (texture())
	{
		texture()->resizeTo(w,h);
	}
}

	
}
