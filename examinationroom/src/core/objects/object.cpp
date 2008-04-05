/*
 *  object.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "object.h"
#include "container.h"
#include "surfaces/abstracttexture.h"

namespace Examination
{
	using namespace Tool;

// Creation
Object::Object()
{
	origin_ = Point(0,0,0);
	setScene(0);
	setParent(0);
	setColor(Color4(1, 1, 1, 1));
	setWireframe(false);
	setShown(true);
}

Object::Object(float x, float y, float z)
{
	origin_.x = x;
	origin_.y = y;
	origin_.z = z;
	setScene(0);
	setParent(0);
}

Object::Object(Point o)
{
	origin_ = o;
	setScene(0);
	setParent(0);
}

Object::~Object()
{
}

Tool::Point Object::position() const
{
	return origin_;
}

void Object::setPosition(Tool::Point p)
{
	origin_ = p;
}

Scene * Object::scene() const
{
	return scene_;
}

void Object::setScene(Scene * s)
{
	scene_ = s;
}

Container * Object::parent() const
{
	return parent_;
}

void Object::setParent(Container * c)
{
	parent_ = c;
}

// Textures
void Object::setTexture(std::tr1::shared_ptr<AbstractTexture> t)
{
	tex_ = t;
}

std::tr1::shared_ptr<AbstractTexture> Object::texture() const
{
	return tex_;
}

Tool::Color4 Object::color() const
{
	return color_;
}

void Object::setColor(Tool::Color4 color)
{
	color_ = color;
}

bool Object::wireframe() const
{
	return wireframe_;
}

void Object::setWireframe(bool flag)
{
	wireframe_ = flag;
}

// Visibility
bool Object::shown() const
{
	return shown_;
}

void Object::setShown(bool shown)
{
	shown_ = shown;
}

bool Object::visible() const
{
	if (parent())
	{
		return shown() && parent()->visible();
	}
	else
	{
		return shown();
	}
}

}
