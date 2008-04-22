/*
 *  cameraproxy.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 28.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef CAMERAPROXY_H
#define CAMERAPROXY_H

#include "luna.h"
#include <memory>

namespace Examination
{
	class Camera;

/**
A proxy class to interface LUA and C++ Camera objects.
 \ingroup LUA
 \author Gerhard Roethlin
*/
class CameraProxy
{
public:
	/**
	From lua, create a new camera proxy object with a new camera with the
	specified properties.
	 \param L	The lua state with the parameter call stack
	*/
	CameraProxy(lua_State *L);

	/**
	From C++, create a new camera proxy object with the passed camera.
	 \param camera	A camera to use for this proxy
	*/
	CameraProxy(std::tr1::shared_ptr<Camera> camera);

	/**
	Destroy the camera proxy.
	*/
	~CameraProxy();

public: // Accessors (get)
	int type(lua_State *L);
	int position(lua_State *L);
	int direction(lua_State *L);
	int up(lua_State *L);
	int separation(lua_State *L);
	int fieldOfView(lua_State *L);
	int paralaxPlane(lua_State *L);

public: // Accessors (set)
	int setType(lua_State *L);
	int setPosition(lua_State *L);
	int setDirection(lua_State *L);
	int setUp(lua_State *L);
	int setSeparation(lua_State *L);
	int setFieldOfView(lua_State *L);
	int setParalaxPlane(lua_State *L);

public:
	std::tr1::shared_ptr<Camera> camera();

private:
	std::tr1::shared_ptr<Camera> camera_;

public: // LUNA
	static const char className[];
	static const Luna<CameraProxy>::RegType Register[];
};

}
#endif
