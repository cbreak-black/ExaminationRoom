/*
 *  objectproxy.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "objectproxy.h"

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

ObjectProxy::ObjectProxy(lua_State *L)
{
	rectangle_ = shared_ptr<Rectangle>(new Rectangle());
	lua_pop(L, 0);
}

ObjectProxy::~ObjectProxy()
{
}
	
int ObjectProxy::dirA(lua_State *L)
{
	checkTop(L, 1);

	Tool::Vector v = rectangle()->dirA();
	lua_pop(L, 1);

	pushVector(L, v);
	return 3;
}

int ObjectProxy::dirB(lua_State *L)
{
	checkTop(L, 1);
	
	Tool::Vector v = rectangle()->dirB();
	lua_pop(L, 1);
	
	pushVector(L, v);
	return 3;
}

int ObjectProxy::setDirA(lua_State *L)
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

int ObjectProxy::setDirB(lua_State *L)
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

int ObjectProxy::position(lua_State *L)
{
	checkTop(L, 1);
	
	Tool::Point p = rectangle()->position();
	lua_pop(L, 1);
	
	pushVector(L, p);
	return 3;
}

int ObjectProxy::setPosition(lua_State *L)
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

int ObjectProxy::setTexCoords(lua_State *L)
{
	return 0;
}

int ObjectProxy::setTexture(lua_State *L)
{
	return 0;
}

shared_ptr<Rectangle> ObjectProxy::rectangle()
{
	return rectangle_;
}

const char ObjectProxy::className[] = "Object";
const Luna<ObjectProxy>::RegType ObjectProxy::Register[] =
{
{ "dirA", &ObjectProxy::dirA },
{ "dirB", &ObjectProxy::dirB },
{ "setDirA", &ObjectProxy::setDirA },
{ "setDirB", &ObjectProxy::setDirB },
{ "position", &ObjectProxy::position },
{ "setPosition", &ObjectProxy::setPosition },
{ "setTexCoords", &ObjectProxy::setTexCoords },
{ "setTexture", &ObjectProxy::setTexture },
{ 0, 0 }
};


}