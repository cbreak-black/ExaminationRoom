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
#include "rectangle.h"

#include <iostream>
#include <time.h>

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
		res = lua_pcall(L_, 0, 0, 0);
		if (res == 0)
			return 0;
	}

	const char *  s = lua_tostring(L_, 1);
	return handleError(res, s);
}

int LuaProxy::runString(const char * code)
{
	int res = luaL_loadstring(L_, code);
	if (res == 0)
	{
		res = lua_pcall(L_, 0, 0, 0);
		if (res == 0)
			return 0;
	}

	const char *  s = lua_tostring(L_, 1);
	return handleError(res, s);
}

// From LUA
int LuaProxy::addRectangle(lua_State *L)
{
	int n = lua_gettop(L);
	if (n != 2)
	{
		lua_pushstring(L, "incorrect number of arguments");
		lua_error(L);
	}
	if (lua_istable(L, 2))
	{
		lua_pushnumber(L, 0);
		lua_gettable(L, 2);
		RectangleProxy ** r = static_cast<RectangleProxy**>(lua_touserdata(L, 3));
		scene_->addObject((*r)->rectangle());
		lua_pop(L, 3);
		return 0;
	}

	lua_pushstring(L, "incorrect argument type");
	lua_error(L);
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

int LuaProxy::log(lua_State *L)
{
//	int n = lua_gettop(L);
//	if (n != 1)
//	{
//		lua_pushstring(L, "incorrect number of arguments");
//		lua_error(L);
//	}
//	else
//	{
		const char *  s = lua_tostring(L, -1);
		char t[32];
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo = gmtime(&rawtime);
		strftime(t, 32, "%Y.%m.%d %H:%M:%S: ", timeinfo);
		std::cout << t << s << std::endl;
//	}
	return 0;
}

void LuaProxy::error(const char * s1, const char * s2)
{
	char t[32];
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = gmtime(&rawtime);
	strftime(t, 32, "%Y.%m.%d %H:%M:%S: ", timeinfo);
	std::cerr << t << s1 << s2 << std::endl;
}

int LuaProxy::handleError(int err, const char * s)
{
	switch (err)
	{
		case LUA_ERRRUN:
			error("Runtime Error", s);
			break;
		case LUA_ERRMEM:
			error("Memory Error", s);
			break;
		case LUA_ERRERR:
			error("Error Error", s);
			break;
		case LUA_ERRFILE:
			error("File Error", s);
			break;
		case LUA_ERRSYNTAX:
			error("Syntax Error", s);
			break;
		default:
			error("Unknown Error", s);
	}
	return err;
}

// LUNA Declarations

const char LuaProxy::className[] = "LuaProxy";
const Luna<LuaProxy>::RegType LuaProxy::Register[] =
{
	{ "addRectangle", &LuaProxy::addRectangle },
	{ "clearScene", &LuaProxy::clearScene },
	{ "setEventListener", &LuaProxy::setEventListener },
	{ "log", &LuaProxy::log },
	{ 0, 0 }
};

	
}
