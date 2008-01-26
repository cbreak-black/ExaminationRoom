/*
 *  luaproxy.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "luaproxy.h"

#include "scene.h"

#include "rectangleproxy.h"

#include <iostream>

namespace Examination
{
// Creation & Destruction	
LuaProxy::LuaProxy(Scene * scene)
{
	scene_ = scene;
	
	L_ = lua_open();
	luaL_openlibs(L_);

	Luna<LuaProxy>::inject(L_, this);
	lua_setglobal(L_, "Scene");
	
	Luna<RectangleProxy>::Register(L_);
}
	
LuaProxy::~LuaProxy()
{
	lua_close(L_);
}

// From C++
int LuaProxy::runFile(const char * path)
{
	int res = luaL_loadfile(L_, path);
	if (res == 0)
	{
		return lua_pcall(L_, 0, 0, 0);
	}
	else
	{
		return res;
	}
}

int LuaProxy::runString(const char * code)
{
	int res = luaL_loadstring(L_, code);
	if (res == 0)
	{
		return lua_pcall(L_, 0, 0, 0);
	}
	else
	{
		return res;
	}
}

// From LUA
int LuaProxy::addRectangle(lua_State *L)
{
	return 0;
}

int LuaProxy::clearScene(lua_State *L)
{
	return 0;
}

int LuaProxy::setEventListener(lua_State *L)
{
	return 0;
}

// LUNA Declarations

const char LuaProxy::className[] = "LuaProxy";
const Luna<LuaProxy>::RegType LuaProxy::Register[] =
{
	{ "addRectangle", &LuaProxy::addRectangle },
	{ "clearScene", &LuaProxy::clearScene },
	{ "setEventListener", &LuaProxy::setEventListener },
	{ 0, 0 }
};

	
}
