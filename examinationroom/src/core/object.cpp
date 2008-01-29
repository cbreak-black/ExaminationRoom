/*
 *  object.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "object.h"

namespace Examination
{
	using namespace Tool;

// Creation
Object::Object()
{
}

Object::Object(float x, float y, float z)
{
	origin_.x = x;
	origin_.y = y;
	origin_.z = z;
}

Object::Object(Point o)
{
	origin_ = o;
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


}
