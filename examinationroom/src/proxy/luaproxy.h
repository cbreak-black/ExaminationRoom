/*
 *  luaproxy.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef LUAPROXY_H
#define LUAPROXY_H

#include "luna.h"

namespace Examination
{
	class Scene;

/**
This proxy class offers an interface between lua and C++.
*/
class LuaProxy
{
public:
	/**
	Creates the proxy and initializes lua.
	*/
	LuaProxy(Scene * scene);
	
	/**
	Cleans up the lua environment.
	*/
	~LuaProxy();

public: // From C++
	int runFile(const char * path);
	int runString(const char * code);

public: // From LUA
	int addRectangle(lua_State *L);
	int clearScene(lua_State *L);
	
	int setEventListener(lua_State *L);
	
private:
	lua_State * L_;
	
	Scene * scene_;

public: // LUNA
	static const char className[];
	static const Luna<LuaProxy>::RegType Register[];
};

	
}
#endif
