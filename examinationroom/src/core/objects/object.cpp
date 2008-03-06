/*
 *  object.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "object.h"
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

}
