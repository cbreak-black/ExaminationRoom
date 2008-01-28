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

using namespace std::tr1;

namespace Examination
{
const char * errArgN = "incorrect number of arguments";
const char * errArgT = "incorrect type of arguments";

bool checkTop(lua_State *L, int num)
{
	int n = lua_gettop(L);
	if (n != num)
	{
		lua_pushstring(L, errArgN);
		lua_error(L);
		return false;
	}
	else
	{
		return true;
	}
}

inline void pushVector(lua_State *L, Tool::Vec3f v)
{
	lua_pushnumber(L, v.x);
	lua_pushnumber(L, v.y);
	lua_pushnumber(L, v.z);	
}

RectangleProxy::RectangleProxy(lua_State *L)
{
	rectangle_ = shared_ptr<Rectangle>(new Rectangle());
	lua_pop(L, 0);
}

RectangleProxy::~RectangleProxy()
{
}
	
int RectangleProxy::dirA(lua_State *L)
{
	checkTop(L, 1);

	Tool::Vector v = rectangle()->dirA();
	lua_pop(L, 1);

	pushVector(L, v);
	return 3;
}

int RectangleProxy::dirB(lua_State *L)
{
	checkTop(L, 1);
	
	Tool::Vector v = rectangle()->dirB();
	lua_pop(L, 1);
	
	pushVector(L, v);
	return 3;
}

int RectangleProxy::setDirA(lua_State *L)
{
	checkTop(L, 4);

	float x, y, z;
	x = lua_tonumber(L, -3);
	y = lua_tonumber(L, -2);
	z = lua_tonumber(L, -1);
	rectangle()->setDirA(Tool::Vector(x,y,z));
	lua_pop(L, 4);
	return 0;
}

int RectangleProxy::setDirB(lua_State *L)
{
	checkTop(L, 4);
	
	float x, y, z;
	x = lua_tonumber(L, -3);
	y = lua_tonumber(L, -2);
	z = lua_tonumber(L, -1);
	rectangle()->setDirB(Tool::Vector(x,y,z));
	lua_pop(L, 4);
	return 0;
}

int RectangleProxy::position(lua_State *L)
{
	checkTop(L, 1);
	
	Tool::Point p = rectangle()->position();
	lua_pop(L, 1);
	
	pushVector(L, p);
	return 3;
}

int RectangleProxy::setPosition(lua_State *L)
{
	checkTop(L, 4);
	
	float x, y, z;
	x = lua_tonumber(L, -3);
	y = lua_tonumber(L, -2);
	z = lua_tonumber(L, -1);
	rectangle()->setPosition(Tool::Point(x,y,z));
	lua_pop(L, 4);
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

shared_ptr<Rectangle> RectangleProxy::rectangle()
{
	return rectangle_;
}

const char RectangleProxy::className[] = "Rectangle";
const Luna<RectangleProxy>::RegType RectangleProxy::Register[] =
{
{ "dirA", &RectangleProxy::dirA },
{ "dirB", &RectangleProxy::dirB },
{ "setDirA", &RectangleProxy::setDirA },
{ "setDirB", &RectangleProxy::setDirB },
{ "position", &RectangleProxy::position },
{ "setPosition", &RectangleProxy::setPosition },
{ "setTexCoords", &RectangleProxy::setTexCoords },
{ "setTexture", &RectangleProxy::setTexture },
{ 0, 0 }
};


}