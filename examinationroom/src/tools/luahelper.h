/*
 *  luahelper.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 28.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef LUAHELPER_H
#define LUAHELPER_H

#include <lua.hpp>
#include "vec.h"

extern const char * errArgN;
extern const char * errArgT;

inline bool checkTop(lua_State *L, int num)
{
	int n = lua_gettop(L);
	if (n != num)
	{
		lua_pushstring(L, errArgN);
		lua_error(L);
		return false;
	}
	else
	{
		return true;
	}
}

inline void pushVector(lua_State *L, Tool::Vec3f v)
{
	lua_pushnumber(L, v.x);
	lua_pushnumber(L, v.y);
	lua_pushnumber(L, v.z);	
}

inline Tool::Vec3f toVector(lua_State *L, int idx)
{
	float x, y, z;
	x = luaL_checknumber(L, idx);
	y = luaL_checknumber(L, idx+1);
	z = luaL_checknumber(L, idx+2);
	return Tool::Vec3f(x,y,z);
}

#endif