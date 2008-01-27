/*
 *  rectangleproxy.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RECTANGLEPROXY_H
#define RECTANGLEPROXY_H

#include "luna.h"

namespace Examination
{
	class Rectangle;

class RectangleProxy
{
public:
	RectangleProxy(lua_State *L);
	~RectangleProxy();

public: // From LUA
	int dirA(lua_State *L);
	int dirB(lua_State *L);
	int setDirA(lua_State *L);
	int setDirB(lua_State *L);

public: // Object
	int position(lua_State *L);
	int setPosition(lua_State *L);

public: // Textures
	int setTexCoords(lua_State *L);
	int setTexture(lua_State *L);
	
public:
	Rectangle * rectangle();
	
private:
	Rectangle * rectangle_;

public: // LUNA
	static const char className[];
	static const Luna<RectangleProxy>::RegType Register[];
};

}
#endif
