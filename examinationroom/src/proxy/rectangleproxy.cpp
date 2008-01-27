/*
 *  rectangleproxy.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "rectangleproxy.h"

#include "rectangle.h"

namespace Examination
{

RectangleProxy::RectangleProxy(lua_State *L)
{
	rectangle_ = new Rectangle(); // Implicit retain
}

RectangleProxy::~RectangleProxy()
{
	rectangle_->release();
}
	
int RectangleProxy::dirA(lua_State *L)
{
	return 3;
}

int RectangleProxy::dirB(lua_State *L)
{
	return 3;
}

int RectangleProxy::setDirA(lua_State *L)
{
	return 0;
}

int RectangleProxy::setDirB(lua_State *L)
{
	return 0;
}

int RectangleProxy::setTexCoords(lua_State *L)
{
	return 0;
}

int RectangleProxy::setTexture(lua_State *L)
{
	return 0;
}

	
const char RectangleProxy::className[] = "Rectangle";
const Luna<RectangleProxy>::RegType RectangleProxy::Register[] =
{
{ "dirA", &RectangleProxy::dirA },
{ "dirB", &RectangleProxy::dirB },
{ "setDirA", &RectangleProxy::setDirA },
{ "setDirB", &RectangleProxy::setDirB },
{ "setTexCoords", &RectangleProxy::setTexCoords },
{ "setTexture", &RectangleProxy::setTexture },
{ 0, 0 }
};


}