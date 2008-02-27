/*
 *  objectproxy.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef OBJECTPROXY_H
#define OBJECTPROXY_H

#include "luna.h"
#include <memory>

namespace Examination
{
	class Rectangle;
	class Parallelepiped;
	class Pixelplane;
	class Text;
	class Object;

class ObjectProxy
{
public:
	ObjectProxy(lua_State *L);

public: // From LUA
	int dirA(lua_State *L);
	int dirB(lua_State *L);
	int dirC(lua_State *L);
	int setDirA(lua_State *L);
	int setDirB(lua_State *L);
	int setDirC(lua_State *L);
	int setSize(lua_State *L);

public: // Object
	int position(lua_State *L);
	int setPosition(lua_State *L);

public: // Textures
	int setTexCoords(lua_State *L);
	int setTexture(lua_State *L);
	int setAutoResize(lua_State *L);

public: // Text
	int text(lua_State *L);
	int setText(lua_State *L);

public:
	std::tr1::shared_ptr<Rectangle> rectangle();
	std::tr1::shared_ptr<Parallelepiped> parallelepiped();
	std::tr1::shared_ptr<Pixelplane> pixelplane();
	std::tr1::shared_ptr<Text> text();
	std::tr1::shared_ptr<Object> object();
	
private:
	std::tr1::shared_ptr<Object> object_;

public: // LUNA
	static const char className[];
	static const Luna<ObjectProxy>::RegType Register[];
};

}
#endif
