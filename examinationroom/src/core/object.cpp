/*
 *  object.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "object.h"
#include "abstracttexture.h"

namespace Examination
{
	using namespace Tool;

// Creation
Object::Object()
{
	autoresize_ = false;
}

Object::Object(float x, float y, float z)
{
	origin_.x = x;
	origin_.y = y;
	origin_.z = z;
	autoresize_ = false;
}

Object::Object(Point o)
{
	origin_ = o;
	autoresize_ = false;
}

Object::~Object()
{
}

Tool::Point Object::position()
{
	return origin_;
}

void Object::setPosition(Tool::Point p)
{
	origin_ = p;
}

void Object::setAutoResize(bool b)
{
	autoresize_ = b;
}

bool Object::autoResize()
{
	return autoresize_;
}

// Textures
void Object::setTexture(std::tr1::shared_ptr<AbstractTexture> t)
{
	tex_ = t;
}

std::tr1::shared_ptr<AbstractTexture> Object::texture()
{
	return tex_;
}

}
