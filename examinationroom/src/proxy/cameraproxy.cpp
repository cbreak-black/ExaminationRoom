/*
 *  cameraproxy.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 28.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "cameraproxy.h"

#include "camera.h"

#include "luahelper.h"

using namespace std::tr1;

namespace Examination
{

const char * cameraTypes[] =
{
	"Perspective",
	"Parallel",
	"Screen",
	0
};

CameraProxy::CameraProxy(lua_State *L)
{
	checkTop(L, 2);
	int opt = luaL_checkoption(L, 1, 0, cameraTypes);
	camera_ = shared_ptr<Camera>(new Camera());
	camera_->setType((Camera::Type)opt);
	lua_pop(L, 2);
}

CameraProxy::CameraProxy(std::tr1::shared_ptr<Camera> camera)
{
	camera_ = camera;
}

CameraProxy::~CameraProxy()
{
}

// Accessors

int CameraProxy::type(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	lua_pushstring(L, cameraTypes[camera_->type()]);
	return 1;
}

int CameraProxy::setType(lua_State *L)
{
	checkTop(L, 2);
	int opt = luaL_checkoption(L, 2, 0, cameraTypes);
	camera_->setType((Camera::Type)opt);
	lua_pop(L, 2);
	return 0;
}

int CameraProxy::position(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	pushVector(L, camera_->position());
	return 3;
}

int CameraProxy::setPosition(lua_State *L)
{
	checkTop(L, 4);
	camera_->setPosition(toVector(L, 2));
	lua_pop(L, 4);
	return 0;
}

int CameraProxy::direction(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	pushVector(L, camera_->direction());
	return 3;
}

int CameraProxy::setDirection(lua_State *L)
{
	checkTop(L, 4);
	camera_->setDirection(toVector(L, 2));
	lua_pop(L, 4);
	return 0;
}

int CameraProxy::up(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	pushVector(L, camera_->up());
	return 3;
}

int CameraProxy::setUp(lua_State *L)
{
	checkTop(L, 4);
	camera_->setUp(toVector(L, 2));
	lua_pop(L, 4);
	return 0;
}

int CameraProxy::separation(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	lua_pushnumber(L, camera_->separation());
	return 1;
}

int CameraProxy::setSeparation(lua_State *L)
{
	checkTop(L, 2);
	camera_->setSeparation(lua_tonumber(L, 2));
	lua_pop(L, 2);
	return 0;
}

int CameraProxy::fieldOfView(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	lua_pushnumber(L, camera_->fieldOfView());
	return 1;
}

int CameraProxy::setFieldOfView(lua_State *L)
{
	checkTop(L, 2);
	camera_->setFieldOfView(lua_tonumber(L, 2));
	lua_pop(L, 2);
	return 0;
}

int CameraProxy::paralaxPlane(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	lua_pushnumber(L, camera_->paralaxPlane());
	return 1;
}

int CameraProxy::setParalaxPlane(lua_State *L)
{
	checkTop(L, 2);
	camera_->setParalaxPlane(lua_tonumber(L, 2));
	lua_pop(L, 2);
	return 0;
}

// Dynamic Casts
std::tr1::shared_ptr<Camera> CameraProxy::camera()
{
	return camera_;
}

const char CameraProxy::className[] = "Camera";
const Luna<CameraProxy>::RegType CameraProxy::Register[] =
{
	{ "type", &CameraProxy::type },
	{ "setType", &CameraProxy::setType },
	{ "position", &CameraProxy::position },
	{ "setPosition", &CameraProxy::setPosition },
	{ "direction", &CameraProxy::direction },
	{ "setDirection", &CameraProxy::setDirection },
	{ "up", &CameraProxy::up },
	{ "setUp", &CameraProxy::setUp },
	{ "separation", &CameraProxy::separation },
	{ "setSeparation", &CameraProxy::setSeparation },
	{ "fieldOfView", &CameraProxy::fieldOfView },
	{ "setFieldOfView", &CameraProxy::setFieldOfView },
	{ "paralaxPlane", &CameraProxy::paralaxPlane },
	{ "setParalaxPlane", &CameraProxy::setParalaxPlane },
	{ 0, 0 }
};

}
