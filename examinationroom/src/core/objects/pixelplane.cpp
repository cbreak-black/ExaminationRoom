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
	setZoom(1,1);
	setName("Pixel Plane");
}

Pixelplane::~Pixelplane()
{
}

// Drawing
void Pixelplane::draw(GLWidget * dest) const
{
	if (shown() && texture())
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

Vec2f Pixelplane::size() const
{
	return Vec2f(width_, height_);
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
		w = uss*width_/zoomFactorX_;
		h = uss*height_/zoomFactorY_;
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

Vec2f Pixelplane::zoom() const
{
	return Vec2f(zoomFactorX_, zoomFactorY_);
}

void Pixelplane::setZoom(float zx, float zy)
{
	zoomFactorX_ = zx;
	zoomFactorY_ = zy;
	if (texture())
	{
		texture()->setZoom(zoomFactorX_, zoomFactorY_);
	}
}

void Pixelplane::setTexture(std::tr1::shared_ptr<AbstractTexture> t)
{
	Object::setTexture(t);
	if (texture())
	{
		texture()->setZoom(zoomFactorX_, zoomFactorY_);
	}
}

// Serialisation
std::string Pixelplane::className() const
{
	return "Pixelplane";
}

std::string Pixelplane::toLua(std::ostream & outStream) const
{
	Object::toLua(outStream);
	outStream << name() << ":" << "setAutoResize(" << (autoResize() ? "true" : "false") << ");\n";
	outStream << name() << ":" << "setSize(" << size().x << ", " << size().y << ");\n";
	outStream << name() << ":" << "setZoom(" << zoom().x << ", " << zoom().y << ");\n";
	return name();
}

}
