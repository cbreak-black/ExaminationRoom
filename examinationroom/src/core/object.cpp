/*******************************************************************************

Examination Room is a flexible user evaluation tool

Copyright (C) 2008 Gerhard Roethlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************************/

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
