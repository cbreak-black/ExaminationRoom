/*
 *  luaproxy.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "luaproxy.h"

#include "scene.h"
#include "camera.h"

#include "objectproxy.h"
#include "textureproxy.h"

#include "rectangle.h"

#include "luahelper.h"

#include <iostream>
#include <QDateTime>

namespace Examination
{

static char * dateTimeFormatString = "yyyy.MM.dd hh:mm:ss.zzz";
static char * logFileFormatString = "yyyy.MM.dd-hh.mm.ss.zzz.'log.txt'";

// Creation & Destruction	
LuaProxy::LuaProxy(Scene * scene)
{
	scene_ = scene;
	
	L_ = lua_open();
	luaL_openlibs(L_);

	Luna<LuaProxy>::inject(L_, this);
	lua_setglobal(L_, "Scene");

	Luna<ObjectProxy>::Register(L_);
	Luna<TextureProxy>::Register(L_);

	QDateTime t = QDateTime::currentDateTime();
	logOutStream_.open(t.toString(logFileFormatString).toAscii());

	lastUpdate_.start();
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

	const char *  s = lua_tostring(L_, -1);
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

	const char *  s = lua_tostring(L_, -1);
	return handleError(res, s);
}

// From LUA
int LuaProxy::addObject(lua_State *L)
{
	checkTop(L, 2);
	
	//luaL_checktype(L, 2, LUA_TTABLE);
	luaL_argcheck(L, lua_istable(L, 2), 2, "Not an Object");
	lua_pushnumber(L, 0);
	lua_gettable(L, -2);

	ObjectProxy ** r = static_cast<ObjectProxy**>(luaL_checkudata(L, -1, ObjectProxy::className));
	scene_->addObject((*r)->object());
	lua_pop(L, 3);

	return 0;
}

int LuaProxy::clearScene(lua_State *L)
{
	scene_->clear();
	lua_pop(L, 0);
	return 0;
}

int LuaProxy::setCameraPos(lua_State *L)
{
	checkTop(L, 4);
	scene_->camera()->setPosition(toVector(L,2));
	lua_pop(L, 4);
	return 0;
}

int LuaProxy::setCameraDir(lua_State *L)
{
	checkTop(L, 4);
	scene_->camera()->setDirection(toVector(L,2));
	lua_pop(L, 4);
	return 0;
}

int LuaProxy::setCameraFoV(lua_State *L)
{
	checkTop(L, 2);
	scene_->camera()->setFieldOfView(lua_tonumber(L,-1));
	lua_pop(L, 2);
	return 0;
}

int LuaProxy::setCameraSep(lua_State *L)
{
	checkTop(L, 2);
	scene_->camera()->setSeperation(lua_tonumber(L,-1));
	lua_pop(L, 2);
	return 0;
}

const char * eventIdx[] =
{
	"update",
	"keyDown",
	"keyUp",
	0
};

int LuaProxy::setEventListener(lua_State *L)
{
	checkTop(L, 3);
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 3, LUA_TFUNCTION);
	int opt = luaL_checkoption(L, 2, 0, eventIdx);
	lua_setfield(L, 1, eventIdx[opt]);
	lua_pop(L, 2);
	return 0;
}

void LuaProxy::onUpdate()
{
	double delta = (double)lastUpdate_.restart() / 1000;
	onEvent(eventIdx[0], delta);
}

void LuaProxy::onKeyDown(char k)
{
	char ts[2];
	ts[0] = k;
	ts[1] = '\0';
	onEvent(eventIdx[1], ts);
}

void LuaProxy::onKeyUp(char k)
{
	char ts[2];
	ts[0] = k;
	ts[1] = '\0';
	onEvent(eventIdx[2], ts);
}

void LuaProxy::onEvent(const char * event, double param)
{
	lua_getfield(L_, LUA_GLOBALSINDEX, "Scene");	// 1
	lua_getfield(L_, 1, event);						// 2
	if (lua_isfunction(L_, 2))
	{
		lua_pushnumber(L_, param);
		int res = lua_pcall(L_, 1, 0, 0);
		if (res != 0)
		{
			// Error
			lua_pop(L_, 1);
		}
		lua_pop(L_, 1);
	}
	else
	{
		lua_pop(L_, 2);
	}
}

void LuaProxy::onEvent(const char * event, char * param)
{
	lua_getfield(L_, LUA_GLOBALSINDEX, "Scene");	// 1
	lua_getfield(L_, 1, event);						// 2
	if (lua_isfunction(L_, 2))
	{
		lua_pushstring(L_, param);
		int res = lua_pcall(L_, 1, 0, 0);
		if (res != 0)
		{
			// Error
			const char *  s = lua_tostring(L_, -1);
			handleError(res, s);
			lua_pop(L_, 1);
		}
		lua_pop(L_, 1);
	}
	else
	{
		lua_pop(L_, 2);
	}
}

int LuaProxy::log(lua_State *L)
{
	const char *  s = lua_tostring(L, -1);
	QDateTime t = QDateTime::currentDateTime();
	logOutStream_ << t.toString(dateTimeFormatString).toStdString() << ": " << s << std::endl;
	lua_pop(L, 1);
	return 0;
}

int LuaProxy::debugLog(lua_State *L)
{
	const char *  s = lua_tostring(L, -1);
	QDateTime t = QDateTime::currentDateTime();
	std::cout << t.toString(dateTimeFormatString).toStdString() << ": " << s << std::endl;
	lua_pop(L, 1);
	return 0;
}

void LuaProxy::error(const char * s1, const char * s2)
{
	QDateTime t = QDateTime::currentDateTime();
	std::cout << t.toString(dateTimeFormatString).toStdString() << ": " << s1 << ": " << s2 << std::endl;
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
	{ "addObject", &LuaProxy::addObject },
	{ "clearScene", &LuaProxy::clearScene },
	{ "setCameraPos", &LuaProxy::setCameraPos },
	{ "setCameraDir", &LuaProxy::setCameraDir },
	{ "setCameraFoV", &LuaProxy::setCameraFoV },
	{ "setCameraSep", &LuaProxy::setCameraSep },
	{ "setEventListener", &LuaProxy::setEventListener },
	{ "log", &LuaProxy::log },
	{ "debugLog", &LuaProxy::debugLog },
	{ 0, 0 }
};

	
}
