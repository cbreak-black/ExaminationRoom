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

#include "objects/object.h"
#include "objects/container.h"
#include "objects/pixelplane.h"
#include "objects/rectangle.h"

#include "surfaces/abstracttexture.h"
#include "surfaces/texture.h"
#include "surfaces/stereogram.h"
#include "surfaces/randomdotstereogram.h"

#include "camera.h"

#include "luabridge.hpp"
#include "luahelper.h"

#include <iostream>
#include <QDateTime>
#include <QApplication>

#include "platform_string.h"

// TODO: Move into it's own file

namespace luabridge
{
	const char * textureStyles[] =
	{
		"convex",
		"concave",
		0
	};
	template <>
	struct tdstack <Examination::Stereogram::Style>
	{
		static void push (lua_State *L, Examination::Stereogram::Style data)
		{
			lua_pushstring(L, textureStyles[data]);
		}
		static Examination::Stereogram::Style get (lua_State *L, int index)
		{
			return (Examination::Stereogram::Style)luaL_checkoption(L, index, 0, textureStyles);
		}
	};
}

namespace Examination
{

static char * dateTimeFormatString = "yyyy.MM.dd hh:mm:ss.zzz";
static char * logFileFormatString = "yyyy.MM.dd-hh.mm.ss.zzz.'log.txt'";

// Keeps "this" from being deallocated
struct null_deleter
{
	void operator()(void const *) const
	{
	}
};

// Creation & Destruction
LuaProxy::LuaProxy(std::tr1::shared_ptr<Scene> scene)
{
	scene_ = scene;
	
	L_ = luaL_newstate();
	luaL_openlibs(L_);

	luabridge::module m(L_);

	m.class_<AbstractTexture>("AbstractTexture")
	.method("width", &AbstractTexture::width)
	.method("height", &AbstractTexture::height)
	.method("resizeTo", &AbstractTexture::resizeTo)
	.method("resizeToOriginal", &AbstractTexture::resizeToOriginal)
	.method("zoom", &AbstractTexture::zoom)
	.method<void (AbstractTexture::*)(const Tool::Vec2f &)>("setZoom", &AbstractTexture::setZoom);

	m.subclass<Texture,AbstractTexture>("Texture")
	.constructor<void (*)(const char *)>()
	.method("path", &Texture::path)
	.method("valid", &Texture::valid);

	m.subclass<Stereogram,AbstractTexture>("Stereogram")
	.constructor<void (*)(std::string,std::string)>()
	.method("offset", &Stereogram::offset)
	.method("setOffset", &Stereogram::setOffset)
	.method("style", &Stereogram::style)
	.method("setStyle", &Stereogram::setStyle);

	m.subclass<RandomdotStereogram,Stereogram>("RandomDot")
	.constructor<void (*)(std::string)>()
	.method("setMaxColor", &RandomdotStereogram::setMaxColor)
	.method("setExclusiveColor", &RandomdotStereogram::setExclusiveColor)
	.method<void (RandomdotStereogram::*)(int,char,char,char)>("setColor", &RandomdotStereogram::setColor)
	.method("resetColor", &RandomdotStereogram::resetColor);

	m.class_<LuaProxy>("Scene")
	.method("addObject", &LuaProxy::addObject)
	.method("removeObject", &LuaProxy::removeObject)
	.method("clearScene", &LuaProxy::clearScene)
	.method("setCameraPos", &LuaProxy::setCameraPos)
	.method("setCameraDir", &LuaProxy::setCameraDir)
	.method("setCameraUp", &LuaProxy::setCameraUp)
	.method("setCameraFoV", &LuaProxy::setCameraFoV)
	.method("setCameraSep", &LuaProxy::setCameraSep)
	.method("setCameraParalaxPlane", &LuaProxy::setCameraParalaxPlane)
	.method("getCameraPos", &LuaProxy::getCameraPos)
	.method("getCameraDir", &LuaProxy::getCameraDir)
	.method("getCameraUp", &LuaProxy::getCameraUp)
	.method("getCameraFoV", &LuaProxy::getCameraFoV)
	.method("getCameraSep", &LuaProxy::getCameraSep)
	.method("getCameraParalaxPlane", &LuaProxy::getCameraParalaxPlane)
	.method("camera", &LuaProxy::camera)
	.method("setCamera", &LuaProxy::setCamera)
	.method("setBackgroundColor", &LuaProxy::setBackgroundColor)
	.method("getSeparationAtPoint", &LuaProxy::getSeparationAtPoint)
	.method("getUnitScreenSize", &LuaProxy::getUnitScreenSize)
	.method("getViewport", &LuaProxy::getViewport)
	.method("beep", &LuaProxy::beep)
	.method("exit", &LuaProxy::exit)
	.method("setEventListener", &LuaProxy::setEventListener)
	.method("log", &LuaProxy::log)
	.method("debugLog", &LuaProxy::debugLog);

	m.class_<Object>("Object")
	.method("position", &Object::position)
	.method("setPosition", &Object::setPosition)
	.method("color", &Object::color)
	.method("setColor", &Object::setColor)
	.method("wireframe", &Object::wireframe)
	.method("setWireframe", &Object::setWireframe)
	.method("drawPriority", &Object::drawPriority)
	.method("setDrawPriority", &Object::setDrawPriority)
	.method("shown", &Object::shown)
	.method("setShown", &Object::setShown)
	.method("visible", &Object::visible)
	.method("texture", &Object::texture)
	.method("setTexture", &Object::setTexture);

	m.subclass<Container,Object>("Container")
	.method("addObject", &Container::addObject)
	.method("removeObject", &Container::removeObject)
	.method("clear", &Container::clear)
	.method("enabled", &Container::enabled)
	.method("setEnabled", &Container::setEnabled);

	m.subclass<Pixelplane,Object>("Pixelplane")
	.constructor<void (*)()>()
	.method("size", &Pixelplane::size)
	.method("setSize", &Pixelplane::setSize)
	.method("autoResize", &Pixelplane::autoResize)
	.method("setAutoResize", &Pixelplane::setAutoResize)
	.method("resizeToCurrent", &Pixelplane::resizeToCurrent)
	.method("resizeTo", &Pixelplane::resizeTo)
	.method("zoom", &Pixelplane::zoom)
	.method("setZoom", &Pixelplane::setZoom);

	m.subclass<Rectangle,Object>("Rectangle")
	.constructor<void (*)()>()
	.method("dirA", &Rectangle::dirA)
	.method("dirB", &Rectangle::dirB)
	.method("setDirA", &Rectangle::setDirA)
	.method("setDirB", &Rectangle::setDirB)
	.method("subdivision", &Rectangle::subdivision)
	.method("setSubdivision", &Rectangle::setSubdivision)
	.method("setTexCoords", &Rectangle::setTexCoords);

	// Add Scene object
	luabridge::tdstack<std::tr1::shared_ptr<LuaProxy> >::push(L_, std::tr1::shared_ptr<LuaProxy>(this, null_deleter()));
	lua_setglobal(L_, "Scene");

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

lua_State * LuaProxy::luaState() const
{
	return L_;
}

// From LUA
/**
Signature: addObject(<Object>)
 \see	Scene::addObject()
*/
void LuaProxy::addObject(std::tr1::shared_ptr<Object> object)
{
	scene_->addObject(object);
}

/**
Signature: removeObject(<Object>)
 \see	Scene::removeObject()
*/
void LuaProxy::removeObject(std::tr1::shared_ptr<Object> object)
{
	scene_->removeObject(object);
}

/**
Signature: clearScene()
 \see	Scene::clearScene()
*/
void LuaProxy::clearScene()
{
	scene_->clear();
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
void LuaProxy::setCameraPos(float x, float y, float z)
{
	Tool::Point p(x, y, z);
	scene_->camera()->setPosition(p);
	char msg[80];
	snprintf(msg, 80, "Camera Position: (%2.1f, %2.1f, %2.1f)", p.x, p.y, p.z);
	log(msg);
}

/**
Signature: setCameraDir(<Number:x>, <Number:y>, <Number:z>)
 \see	Camera::setDirection()
*/
void LuaProxy::setCameraDir(float x, float y, float z)
{
	Tool::Point p(x, y, z);
	scene_->camera()->setDirection(p);
	char msg[80];
	snprintf(msg, 80, "Camera Direction: (%2.1f, %2.1f, %2.1f)", p.x, p.y, p.z);
	log(msg);
}

/**
Signature: setCameraUp(<Number:x>, <Number:y>, <Number:z>)
 \see	Camera::setUp()
*/
void LuaProxy::setCameraUp(float x, float y, float z)
{
	Tool::Point p(x, y, z);
	scene_->camera()->setUp(p);
	char msg[80];
	snprintf(msg, 80, "Camera Up: (%2.1f, %2.1f, %2.1f)", p.x, p.y, p.z);
	log(msg);
}

/**
Signature: setCameraFov(<Number:fov>)
 \see	Camera::setFieldOfView()
*/
void LuaProxy::setCameraFoV(float fov)
{
	scene_->camera()->setFieldOfView(fov);
	char msg[80];
	snprintf(msg, 80, "Camera Field of View: %2.2f deg", fov);
	log(msg);
}

/**
Signature: setCameraSeparation(<Number:sep>)
 \see	Camera::setSeparation()
*/
void LuaProxy::setCameraSep(float sep)
{
	scene_->camera()->setSeparation(sep);
	char msg[80];
	snprintf(msg, 80, "Camera Separation: %2.3f", sep);
	log(msg);
}

/**
Signature: setCameraParalaxPlane(<Number:dist>)
 \see	Camera::setParalaxPlane()
*/
void LuaProxy::setCameraParalaxPlane(float pp)
{
	scene_->camera()->setParalaxPlane(pp);
	char msg[80];
	snprintf(msg, 80, "Camera Paralax Plane: %2.2f", pp);
	log(msg);
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
std::tr1::shared_ptr<Camera> LuaProxy::camera() const
{
	return scene_->camera();
}

/**
Signature: setCamera(<Camera>)
 \see	Camera
 \see	Camera::setCamera()
*/
void LuaProxy::setCamera(std::tr1::shared_ptr<Camera> camera)
{
	scene_->setCamera(camera);
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
/**
Signature: beep()
*/
int LuaProxy::beep(lua_State *L)
{
	QApplication::beep();
	lua_settop(L, 0);
	return 0;
}

/**
Signature: exit()
Signature: exit(<Number:result>)
Exits the application with the given result value
*/
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
/**
The supported events that lua code can be triggered by.
*/
const char * eventIdx[] =
{
	"update",
	"keyDown",
	"keyUp",
	"quit",
	0
};

/**
Signature: setEventListener(<String:eventIdx>, <Function:handler>)
 \see	eventIdx
*/
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
	
}
