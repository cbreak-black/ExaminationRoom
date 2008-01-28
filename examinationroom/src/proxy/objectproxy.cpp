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

#include "textureproxy.h"

#include "luahelper.h"

using namespace std::tr1;

namespace Examination
{

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
	checkTop(L, 9);
	
	float llx = lua_tonumber(L, -8);
	float lly = lua_tonumber(L, -7);
	float ulx = lua_tonumber(L, -6);
	float uly = lua_tonumber(L, -5);
	float lrx = lua_tonumber(L, -4);
	float lry = lua_tonumber(L, -3);
	float urx = lua_tonumber(L, -2);
	float ury = lua_tonumber(L, -1);
	
	rectangle()->setTexCoords(llx,lly, ulx, uly,
							  lrx, lry, urx, ury);

	lua_pop(L, 9);
	return 0;
}

int ObjectProxy::setTexture(lua_State *L)
{
	checkTop(L, 2);

	if (lua_istable(L, 2))
	{
		lua_pushnumber(L, 0);
		lua_gettable(L, -2);
		TextureProxy ** t = static_cast<TextureProxy**>(luaL_checkudata(L, -1, TextureProxy::className));
		if (t)
		{
			rectangle()->setTexture((*t)->texture());
			lua_pop(L, 2);
			return 0;
		}
	}

	lua_pushstring(L, errArgT);
	lua_error(L);
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
