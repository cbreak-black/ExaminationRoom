/*
 *  pixelplane.cpp
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

#include "parameter/parameterpixelplane.h"

#include "luabridge.hpp"
#include "luahelper.h"

namespace Examination
{
	using namespace Tool;
	
Pixelplane::Pixelplane()
{
	setAutoResize(false);
	setSize(1,1);
	setZoom(1,1);
	setName("pixelplane");
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
	objectWillChange();
	width_ = w;
	height_ = h;
	objectDidChange();
}

void Pixelplane::setAutoResize(bool b)
{
	objectWillChange();
	autoresize_ = b;
	objectDidChange();
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
	objectWillChange();
	zoomFactorX_ = zx;
	zoomFactorY_ = zy;
	if (texture())
	{
		texture()->setZoom(zoomFactorX_, zoomFactorY_);
	}
	objectDidChange();
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
const char * Pixelplane::className_ = "Pixelplane";

std::string Pixelplane::className() const
{
	return Pixelplane::className_;
}

std::string Pixelplane::toLua(std::ostream & outStream) const
{
	Object::toLua(outStream);
	outStream << name() << ":" << "setAutoResize(" << (autoResize() ? "true" : "false") << ");\n";
	outStream << name() << ":" << "setSize(" << size().x << ", " << size().y << ");\n";
	outStream << name() << ":" << "setZoom(" << zoom().x << ", " << zoom().y << ");\n";
	return name();
}

// LUA
void Pixelplane::registerLuaApi(luabridge::module * m)
{
	m->subclass<Pixelplane,Object>(Pixelplane::className_)
	.constructor<void (*)()>()
	.method("size", &Pixelplane::size)
	.method("setSize", &Pixelplane::setSize)
	.method("autoResize", &Pixelplane::autoResize)
	.method("setAutoResize", &Pixelplane::setAutoResize)
	.method("resizeToCurrent", &Pixelplane::resizeToCurrent)
	.method("resizeTo", &Pixelplane::resizeTo)
	.method("zoom", &Pixelplane::zoom)
	.method("setZoom", &Pixelplane::setZoom);
}

std::tr1::shared_ptr<ParameterObject> Pixelplane::createDialog()
{
	return std::tr1::shared_ptr<ParameterObject>(new ParameterPixelplane(sharedPtr()));
}

}
