/*
 *  luaproxy.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "luaproxy.h"

#include "program.h"
#include "scene.h"
#include "camera.h"
#include "screenproject.h"

#include "surfaces/abstracttexture.h"
#include "surfaces/texture.h"
#include "surfaces/stereogram.h"
#include "surfaces/randomdotstereogram.h"
#include "surfaces/patternstereogram.h"

#include "luabridge.hpp"
#include "luahelper.h"

#include "logtool.h"

#include <QApplication>
#include <QFile>

#include "platform_string.h"

// TODO: Move into it's own file

namespace luabridge
{
	const char * filterType[] =
	{
		"nearest",
		"linear",
		0
	};
	template <>
	struct tdstack <Examination::AbstractTexture::FilterType>
	{
		static void push (lua_State *L, Examination::AbstractTexture::FilterType data)
		{
			lua_pushstring(L, filterType[data]);
		}
		static Examination::AbstractTexture::FilterType get (lua_State *L, int index)
		{
			return static_cast<Examination::AbstractTexture::FilterType>(luaL_checkoption(L, index, 0, filterType));
		}
	};
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
			return static_cast<Examination::Stereogram::Style>(luaL_checkoption(L, index, 0, textureStyles));
		}
	};
	const char * cameraTypes[] =
	{
		"Perspective",
		"Parallel",
		"Screen",
		0
	};
	template <>
	struct tdstack <Examination::Camera::Type>
	{
		static void push (lua_State *L, Examination::Camera::Type data)
		{
			lua_pushstring(L, cameraTypes[data]);
		}
		static Examination::Camera::Type get (lua_State *L, int index)
		{
			return static_cast<Examination::Camera::Type>(luaL_checkoption(L, index, 0, cameraTypes));
		}
	};
}

namespace Examination
{

/**
Keeps "this" from being deallocated
*/
struct null_deleter
{
	void operator()(void const *) const
	{
	}
};

// Creation & Destruction
LuaProxy::LuaProxy()
{
	L_ = luaL_newstate();
	luaL_openlibs(L_);

	luabridge::module m(L_);

	m.class_<Camera>("Camera")
	.constructor<void (*)()>()
	.method("type", &Camera::type)
	.method("setType", &Camera::setType)
	.method("position", &Camera::position)
	.method("setPosition", &Camera::setPosition)
	.method("direction", &Camera::direction)
	.method("setDirection", &Camera::setDirection)
	.method("up", &Camera::up)
	.method("setUp", &Camera::setUp)
	.method("separation", &Camera::separation)
	.method("setSeparation", &Camera::setSeparation)
	.method("fieldOfView", &Camera::fieldOfView)
	.method("setFieldOfView", &Camera::setFieldOfView)
	.method("paralaxPlane", &Camera::paralaxPlane)
	.method("setParalaxPlane", &Camera::setParalaxPlane)
	.method<float (Camera::*)(Tool::Point) const>("unitScreenSize", &Camera::unitScreenSize)
	.method("separationAtPoint", &Camera::separationAtPoint);

	m.class_<AbstractTexture>("AbstractTexture")
	.method("width", &AbstractTexture::width)
	.method("height", &AbstractTexture::height)
	.method("resizeTo", &AbstractTexture::resizeTo)
	.method("resizeToOriginal", &AbstractTexture::resizeToOriginal)
	.method("zoom", &AbstractTexture::zoom)
	.method<void (AbstractTexture::*)(float, float)>("setZoom", &AbstractTexture::setZoom)
	.method("filterType", &AbstractTexture::filterType)
	.method("setFilterType", &AbstractTexture::setFilterType);

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
	.method("maxColor", &RandomdotStereogram::maxColor)
	.method("exclusiveColor", &RandomdotStereogram::exclusiveColor)
	.method("setMaxColor", &RandomdotStereogram::setMaxColor)
	.method("setExclusiveColor", &RandomdotStereogram::setExclusiveColor)
	.method<void (RandomdotStereogram::*)(int,char,char,char)>("setColor", &RandomdotStereogram::setColor)
	.method("resetColor", &RandomdotStereogram::resetColor);

	m.subclass<PatternStereogram,Stereogram>("Pattern")
	.constructor<void (*)(std::string,std::string,std::string)>();

	m.class_<LuaProxy>("Scene")
	.method("loadLua", &LuaProxy::loadLua)
	.method("addObject", &LuaProxy::addObject)
	.method("removeObject", &LuaProxy::removeObject)
	.method("clearScene", &LuaProxy::clearScene)
	.method("split", &LuaProxy::split)
	.method("merge", &LuaProxy::merge)
	.method("clone", &LuaProxy::clone)
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

	// Add Scene object
	luabridge::tdstack<std::tr1::shared_ptr<LuaProxy> >::push(L_, std::tr1::shared_ptr<LuaProxy>(this, null_deleter()));
	lua_setglobal(L_, "Scene");

	// Load ER Libraries
	runResource(":/stringwriter.lua");
	runResource(":/print.lua");
	runResource(":/persistence.lua");
	runResource(":/event.lua");
	runResource(":/misc.lua");

	lastUpdate_.start();
}

LuaProxy::~LuaProxy()
{
	lua_close(L_);
}

std::tr1::shared_ptr<Program> LuaProxy::program() const
{
	return program_.lock();
}

void LuaProxy::setProgram(std::tr1::shared_ptr<Program> program)
{
	program_ = program;
}

std::tr1::shared_ptr<Scene> LuaProxy::scene() const
{
	std::tr1::shared_ptr<Program> p = program_.lock();
	if (p)
		return p->scene();
	else
		return std::tr1::shared_ptr<Scene>(); // Invalid
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
	lua_pop(L_, 1); // Clean up stack
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
	lua_pop(L_, 1); // Clean up stack
	return handleError(res, s);
}

int LuaProxy::runResource(const char * path)
{
	QFile qfile(path);
	if (qfile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString qstr = qfile.readAll();
		if (!qstr.isNull())
		{
			QByteArray qba = qstr.toAscii();
			const char * src = qba.data();
			int res = luaL_loadbuffer(L_, src, strlen(src), path);
			if (res == 0)
			{
				res = lua_pcall(L_, 0, 0, 0);
				if (res == 0)
					return 0;
			}
			const char *  s = lua_tostring(L_, -1);
			lua_pop(L_, 1); // Clean up stack
			return handleError(res, s);
		}
		return -1; // nothing was read
	}
	return -2; // file could not be opened
}

lua_State * LuaProxy::luaState() const
{
	return L_;
}

// From LUA
/**
Signature: loadLua(<Path>)
 \see	Program::loadLua()
 */
void LuaProxy::loadLua(const std::string & path)
{
	std::tr1::shared_ptr<Program> p = program();
	if (p)
		p->loadLua(path);
}

/**
Signature: addObject(<Object>)
 \see	Scene::addObject()
*/
void LuaProxy::addObject(std::tr1::shared_ptr<Object> object)
{
	scene()->addObject(object);
}

/**
Signature: removeObject(<Object>)
 \see	Scene::removeObject()
*/
void LuaProxy::removeObject(std::tr1::shared_ptr<Object> object)
{
	scene()->removeObject(object);
}

/**
Signature: clearScene()
 \see	Scene::clearScene()
*/
void LuaProxy::clearScene()
{
	scene()->clear();
}

/**
Signature: split()
 \see Container::split()
*/
std::tr1::shared_ptr<Container> LuaProxy::split()
{
	return scene()->split();
}

/**
Signature: merge(<Container>)
 \see Container::merge()
*/
void LuaProxy::merge(std::tr1::shared_ptr<Container> c)
{
	scene()->merge(c);
}

/**
Signature: clone()
 \see Container::clone()
*/
std::tr1::shared_ptr<Container> LuaProxy::clone()
{
	return std::tr1::dynamic_pointer_cast<Container>(scene()->clone());
}

/**
Signature: setBackground(<red>, <green>, <blue>, <alpha>)
Each component can have values ranging from 0 to 255.
 \see	Scene::setBackgroundColor()
*/
void LuaProxy::setBackgroundColor(char red, char green, char blue, char alpha)
{
	scene()->setBackgroundColor(red, green, blue, alpha);
}

/**
Signature: setCameraPos({<Number:x>, <Number:y>, <Number:z>})
 \see	Camera::setPosition()
*/
void LuaProxy::setCameraPos(float x, float y, float z)
{
	Tool::Point p(x, y, z);
	scene()->camera()->setPosition(p);
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
	scene()->camera()->setDirection(p);
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
	scene()->camera()->setUp(p);
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
	scene()->camera()->setFieldOfView(fov);
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
	scene()->camera()->setSeparation(sep);
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
	scene()->camera()->setParalaxPlane(pp);
	char msg[80];
	snprintf(msg, 80, "Camera Paralax Plane: %2.2f", pp);
	log(msg);
}

/**
Signature: {x, y, z} = getCameraPos()
 \see	Camera::position()
*/
Tool::Point LuaProxy::getCameraPos()
{
	return scene()->camera()->position();
}

/**
Signature: {x, y, z} = getCameraDir()
 \see	Camera::direction()
*/
Tool::Vector LuaProxy::getCameraDir()
{
	return scene()->camera()->direction();
}

/**
Signature: {x, y, z} = getCameraUp()
 \see	Camera::up()
*/
Tool::Vector LuaProxy::getCameraUp()
{
	return scene()->camera()->up();
}

/**
Signature: fov = getCameraFoV()
 \see	Camera::fieldOfView()
*/
float LuaProxy::getCameraFoV()
{
	return scene()->camera()->fieldOfView();
}

/**
Signature: sep = getCameraSep()
 \see	Camera::separation()
*/
float LuaProxy::getCameraSep()
{
	return scene()->camera()->separation();
}

/**
Signature: dist = getCameraParalaxPlane()
 \see	Camera::paralaxPlane()
*/
float LuaProxy::getCameraParalaxPlane()
{
	return scene()->camera()->paralaxPlane();
}

/**
Signature: cam = camera()
 \see	Camera
 \see	Scene::camera()
*/
std::tr1::shared_ptr<Camera> LuaProxy::camera() const
{
	return scene()->camera();
}

/**
Signature: setCamera(<Camera>)
 \see	Camera
 \see	Camera::setCamera()
*/
void LuaProxy::setCamera(std::tr1::shared_ptr<Camera> camera)
{
	scene()->setCamera(camera);
}

/**
Signature: sep = getSeparationAtPoint({<Number:x>, <Number:y>, <Number:z>})
 \see	Camera::separationAtPoint()
*/
float LuaProxy::getSeparationAtPoint(Tool::Point p)
{
	return scene()->camera()->separationAtPoint(p);
}

/**
Signature: uss = getUnitScreenSize({<Number:x>, <Number:y>, <Number:z>})
 \see	Camera::unitScreenSize()
*/
float LuaProxy::getUnitScreenSize(Tool::Point p)
{
	return scene()->camera()->unitScreenSize(p);
}

/**
Signature: x, y, w, h = getViewport()
 \see	Camera::screenProject()
 \see	ScreenProject::viewport()
 \warning Old Code, Revisit
*/
int LuaProxy::getViewport(lua_State *L)
{
	checkTop(L, 1);
	// Could be right side too, the viewport should be the same...
	long int * vp_ = scene()->camera()->screenProject(GLWidget::left)->viewport();
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
void LuaProxy::beep()
{
	QApplication::beep();
}

/**
Signature: exit()
Signature: exit(<Number:result>) // Unimplemented
Exits the application with the given result value
*/
void LuaProxy::exit()
{
	QCoreApplication::exit(0);
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
 \warning Old Code, Revisit
*/
int LuaProxy::setEventListener(lua_State *L)
{
	checkTop(L, 3);
	//luaL_checktype(L, 3, LUA_TFUNCTION);
	int opt = luaL_checkoption(L, 2, 0, eventIdx);
	int t = lua_type(L, 3);

	if (lua_getmetatable(L, 1)) // 4
	{
		lua_pushstring(L, eventIdx[opt]); // 5 -- key
		if (t == LUA_TFUNCTION)
			lua_pushvalue(L, 3); // 6 -- value
		else
			lua_pushnil(L); // 6 -- value
		lua_settable(L, 4); // 4 -- meta[key] = value, pops key, value
		lua_pop(L, 1); // 3 -- pops meta
	}
	lua_pop(L, 3);
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
	LogTool::logError(s1, "Lua", s2);
	std::tr1::shared_ptr<Program> p = program();
	if (p)
		p->writeError(s1, s2);
}

void LuaProxy::log(const char * msg)
{
	LogTool::logMessage("Lua", msg);
	std::tr1::shared_ptr<Program> p = program();
	if (p)
		p->writeLog(msg);
}

void LuaProxy::debugLog(const char * msg)
{
	LogTool::logError("DEBUG", "Lua", msg);
	std::tr1::shared_ptr<Program> p = program();
	if (p)
		p->writeError("DEBUG", msg);
}
	
int LuaProxy::handleError(int err, const char * s)
{
	switch (err)
	{
		case LUA_ERRRUN:
			error("LUA_RUNTIME", s);
			break;
		case LUA_ERRMEM:
			error("LUA_MEMORY", s);
			break;
		case LUA_ERRERR:
			error("LUA_ERROR", s);
			break;
		case LUA_ERRFILE:
			error("LUA_FILE", s);
			break;
		case LUA_ERRSYNTAX:
			error("LUA_SYNTAX", s);
			break;
		default:
			error("LUA_UNKNOWN", s);
	}
	return err;
}
	
}
