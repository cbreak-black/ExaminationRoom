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
#include "screenproject.h"

#include "objectproxy.h"
#include "textureproxy.h"
#include "cameraproxy.h"

#include "luahelper.h"

#include <iostream>
#include <QDateTime>
#include <QCoreApplication>

namespace Examination
{

static char * dateTimeFormatString = "yyyy.MM.dd hh:mm:ss.zzz";
static char * logFileFormatString = "yyyy.MM.dd-hh.mm.ss.zzz.'log.txt'";

// Creation & Destruction
LuaProxy::LuaProxy(std::tr1::shared_ptr<Scene> scene)
{
	scene_ = scene;
	
	L_ = lua_open();
	luaL_openlibs(L_);

	Luna<LuaProxy>::inject(L_, this);
	lua_setglobal(L_, "Scene");

	Luna<ObjectProxy>::Register(L_);
	Luna<TextureProxy>::Register(L_);
	Luna<CameraProxy>::Register(L_);

	QDateTime t = QDateTime::currentDateTime();
	logOutStream_.open(t.toString(logFileFormatString).toAscii());

	lastUpdate_.start();
}

LuaProxy::~LuaProxy()
{
	onQuit();
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
/**
Signature: addObject(<Object>)
 \see	Scene::addObject()
*/
int LuaProxy::addObject(lua_State *L)
{
	checkTop(L, 2);
	ObjectProxy * o = Luna<ObjectProxy>::extract(L, 2);
	scene_->addObject(o->object());
	lua_pop(L, 2);
	return 0;
}

/**
Signature: removeObject(<Object>)
 \see	Scene::removeObject()
*/
int LuaProxy::removeObject(lua_State *L)
{
	checkTop(L, 2);

	luaL_argcheck(L, lua_istable(L, 2), 2, "Not an Object");
	lua_pushnumber(L, 0);
	lua_gettable(L, -2);

	ObjectProxy ** r = static_cast<ObjectProxy**>(luaL_checkudata(L, -1, ObjectProxy::className));
	scene_->removeObject((*r)->object());
	lua_pop(L, 3);

	return 0;
}

/**
Signature: clearScene()
 \see	Scene::clearScene()
*/
int LuaProxy::clearScene(lua_State *L)
{
	scene_->clear();
	lua_pop(L, 0);
	return 0;
}

/**
Signature: setBackground(<red>, <green>, <blue>, <alpha>)
Each component can have values ranging from 0 to 255.
 \see	Scene::setBackgroundColor()
*/
int LuaProxy::setBackgroundColor(lua_State *L)
{
	checkTop(L, 5);
	scene_->setBackgroundColor(lua_tonumber(L, 2),
							   lua_tonumber(L, 3),
							   lua_tonumber(L, 4),
							   lua_tonumber(L, 5));
	lua_pop(L, 5);
	return 0;
}

/**
Signature: setCameraPos(<Number:x>, <Number:y>, <Number:z>)
 \see	Camera::setPosition()
*/
int LuaProxy::setCameraPos(lua_State *L)
{
	checkTop(L, 4);
	Tool::Point p = toVector(L,2);
	scene_->camera()->setPosition(p);
	lua_pop(L, 4);
	char msg[80];
	snprintf(msg, 80, "Camera Position: (%2.1f, %2.1f, %2.1f)", p.x, p.y, p.z);
	log(msg);
	return 0;
}

/**
Signature: setCameraDir(<Number:x>, <Number:y>, <Number:z>)
 \see	Camera::setDirection()
*/
int LuaProxy::setCameraDir(lua_State *L)
{
	checkTop(L, 4);
	Tool::Point p = toVector(L,2);
	scene_->camera()->setDirection(p);
	lua_pop(L, 4);
	char msg[80];
	snprintf(msg, 80, "Camera Direction: (%2.1f, %2.1f, %2.1f)", p.x, p.y, p.z);
	log(msg);
	return 0;
}

/**
Signature: setCameraUp(<Number:x>, <Number:y>, <Number:z>)
 \see	Camera::setUp()
*/
int LuaProxy::setCameraUp(lua_State *L)
{
	checkTop(L, 4);
	Tool::Point p = toVector(L,2);
	scene_->camera()->setUp(p);
	lua_pop(L, 4);
	char msg[80];
	snprintf(msg, 80, "Camera Up: (%2.1f, %2.1f, %2.1f)", p.x, p.y, p.z);
	log(msg);
	return 0;
}

/**
Signature: setCameraFov(<Number:fov>)
 \see	Camera::setFieldOfView()
*/
int LuaProxy::setCameraFoV(lua_State *L)
{
	checkTop(L, 2);
	float fov = lua_tonumber(L,2);
	scene_->camera()->setFieldOfView(fov);
	lua_pop(L, 2);
	char msg[80];
	snprintf(msg, 80, "Camera Field of View: %2.2f deg", fov);
	log(msg);
	return 0;
}

/**
Signature: setCameraSeparation(<Number:sep>)
 \see	Camera::setSeparation()
*/
int LuaProxy::setCameraSep(lua_State *L)
{
	checkTop(L, 2);
	float sep = lua_tonumber(L,2);
	scene_->camera()->setSeparation(sep);
	lua_pop(L, 2);
	char msg[80];
	snprintf(msg, 80, "Camera Separation: %2.3f", sep);
	log(msg);
	return 0;
}

/**
Signature: setCameraParalaxPlane(<Number:dist>)
 \see	Camera::setParalaxPlane()
*/
int LuaProxy::setCameraParalaxPlane(lua_State *L)
{
	checkTop(L, 2);
	float pp = lua_tonumber(L,2);
	scene_->camera()->setParalaxPlane(pp);
	lua_pop(L, 2);
	char msg[80];
	snprintf(msg, 80, "Camera Paralax Plane: %2.2f", pp);
	log(msg);
	return 0;
}

/**
Signature: x, y, z = getCameraPos()
 \see	Camera::position()
*/
int LuaProxy::getCameraPos(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	pushVector(L, scene_->camera()->position());
	return 3;
}

/**
Signature: x, y, z = getCameraDir()
 \see	Camera::direction()
*/
int LuaProxy::getCameraDir(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	pushVector(L, scene_->camera()->direction());
	return 3;
}

/**
Signature: x, y, z = getCameraUp()
 \see	Camera::up()
*/
int LuaProxy::getCameraUp(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	pushVector(L, scene_->camera()->up());
	return 3;
}

/**
Signature: fov = getCameraFoV()
 \see	Camera::fieldOfView()
*/
int LuaProxy::getCameraFoV(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	lua_pushnumber(L, scene_->camera()->fieldOfView());
	return 1;
}

/**
Signature: sep = getCameraSep()
 \see	Camera::separation()
*/
int LuaProxy::getCameraSep(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	lua_pushnumber(L, scene_->camera()->separation());
	return 1;
}

/**
Signature: dist = getCameraParalaxPlane()
 \see	Camera::paralaxPlane()
*/
int LuaProxy::getCameraParalaxPlane(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	lua_pushnumber(L, scene_->camera()->paralaxPlane());
	return 1;
}

/**
Signature: cam = camera()
 \see	Camera
 \see	Scene::camera()
*/
int LuaProxy::camera(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	CameraProxy * cp = new CameraProxy(scene_->camera());
	Luna<CameraProxy>::inject(L, cp);
	return 1;
}

/**
Signature: setCamera(<Camera>)
 \see	Camera
 \see	Camera::setCamera()
*/
int LuaProxy::setCamera(lua_State *L)
{
	checkTop(L, 2);
	CameraProxy * cp = Luna<CameraProxy>::extract(L, 2);
	scene_->setCamera(cp->camera());
	lua_pop(L, 2);
	return 0;
}

/**
Signature: sep = getSeparationAtPoint(<Number:x>, <Number:y>, <Number:z>)
 \see	Camera::separationAtPoint()
*/
int LuaProxy::getSeparationAtPoint(lua_State *L)
{
	checkTop(L, 4);
	float sep = scene_->camera()->separationAtPoint(toVector(L,2));
	lua_pop(L, 4);
	lua_pushnumber(L, sep);
	return 1;
}

/**
Signature: uss = getUnitScreenSize(<Number:x>, <Number:y>, <Number:z>)
 \see	Camera::unitScreenSize()
*/
int LuaProxy::getUnitScreenSize(lua_State *L)
{
	checkTop(L, 4);
	int uss = scene_->camera()->unitScreenSize(toVector(L,2));
	lua_pop(L, 4);
	lua_pushinteger(L, uss);
	return 1;
}

/**
Signature: x, y, w, h = getViewport()
 \see	Camera::screenProject()
 \see	ScreenProject::viewport()
*/
int LuaProxy::getViewport(lua_State *L)
{
	checkTop(L, 1);
	// Could be right side too, the viewport should be the same...
	long int * vp_ = scene_->camera()->screenProject(GLWidget::left)->viewport();
	lua_pop(L, 1);
	lua_pushinteger(L, vp_[0]);
	lua_pushinteger(L, vp_[1]);
	lua_pushinteger(L, vp_[2]);
	lua_pushinteger(L, vp_[3]);
	return 4;
}

// Low level wrapper
int LuaProxy::exit(lua_State *L)
{
	int res = 0;
	if (lua_isnumber(L, 2))
	{
		res = lua_tonumber(L, 2);
	}
	QCoreApplication::exit(res);
	lua_settop(L, 0);
	return 0;
}

// Event Stuff
const char * eventIdx[] =
{
	"update",
	"keyDown",
	"keyUp",
	"quit",
	0
};

int LuaProxy::setEventListener(lua_State *L)
{
	checkTop(L, 3);
	luaL_checktype(L, 1, LUA_TTABLE);
	//luaL_checktype(L, 3, LUA_TFUNCTION);
	int opt = luaL_checkoption(L, 2, 0, eventIdx);
	int t = lua_type(L, 3);
	if (t == LUA_TFUNCTION)
	{
		lua_setfield(L, 1, eventIdx[opt]);
		lua_pop(L, 2);
	}
	else
	{
		lua_pushnil(L);
		lua_setfield(L, 1, eventIdx[opt]);
		lua_pop(L, 3);
	}
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
	lua_gc(L_, LUA_GCSTEP, 8);
}

void LuaProxy::onKeyUp(char k)
{
	char ts[2];
	ts[0] = k;
	ts[1] = '\0';
	onEvent(eventIdx[2], ts);
	lua_gc(L_, LUA_GCSTEP, 8);
}

void LuaProxy::onQuit()
{
	double reserved = 0;
	onEvent(eventIdx[3], reserved);
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
	log(s);
	lua_pop(L, 1);
	return 0;
}

int LuaProxy::debugLog(lua_State *L)
{
	const char *  s = lua_tostring(L, -1);
	debugLog(s);
	lua_pop(L, 1);
	return 0;
}

void LuaProxy::error(const char * s1, const char * s2)
{
	QDateTime t = QDateTime::currentDateTime();
	std::cout << t.toString(dateTimeFormatString).toStdString() << ": " << s1 << ": " << s2 << std::endl;
}

void LuaProxy::log(const char * msg)
{
	QDateTime t = QDateTime::currentDateTime();
	logOutStream_ << t.toString(dateTimeFormatString).toStdString() << ": " << msg << std::endl;
}

void LuaProxy::debugLog(const char * msg)
{
	QDateTime t = QDateTime::currentDateTime();
	std::cout << t.toString(dateTimeFormatString).toStdString() << ": " << msg << std::endl;
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
	{ "removeObject", &LuaProxy::removeObject },
	{ "clearScene", &LuaProxy::clearScene },
	{ "setCameraPos", &LuaProxy::setCameraPos },
	{ "setCameraDir", &LuaProxy::setCameraDir },
	{ "setCameraUp", &LuaProxy::setCameraUp },
	{ "setCameraFoV", &LuaProxy::setCameraFoV },
	{ "setCameraSep", &LuaProxy::setCameraSep },
	{ "setCameraParalaxPlane", &LuaProxy::setCameraParalaxPlane },
	{ "getCameraPos", &LuaProxy::getCameraPos },
	{ "getCameraDir", &LuaProxy::getCameraDir },
	{ "getCameraUp", &LuaProxy::getCameraUp },
	{ "getCameraFoV", &LuaProxy::getCameraFoV },
	{ "getCameraSep", &LuaProxy::getCameraSep },
	{ "getCameraParalaxPlane", &LuaProxy::getCameraParalaxPlane },
	{ "camera", &LuaProxy::camera },
	{ "setCamera", &LuaProxy::setCamera },
	{ "setBackgroundColor", &LuaProxy::setBackgroundColor },
	{ "getSeparationAtPoint", &LuaProxy::getSeparationAtPoint },
	{ "getUnitScreenSize", &LuaProxy::getUnitScreenSize },
	{ "getViewport", &LuaProxy::getViewport },
	{ "exit", &LuaProxy::exit },
	{ "setEventListener", &LuaProxy::setEventListener },
	{ "log", &LuaProxy::log },
	{ "debugLog", &LuaProxy::debugLog },
	{ 0, 0 }
};

	
}
