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

inline bool checkTop(lua_State *L, int num)
{
	int n = lua_gettop(L);
	if (n != num)
	{
		luaL_error(L, errArgN, n, num);
		return false;
	}
	else
	{
		return true;
	}
}

inline void pushVector2(lua_State *L, Tool::Vec2f v)
{
	lua_pushnumber(L, v.x);
	lua_pushnumber(L, v.y);
}

inline void pushVector(lua_State *L, Tool::Vec3f v)
{
	lua_pushnumber(L, v.x);
	lua_pushnumber(L, v.y);
	lua_pushnumber(L, v.z);
}

inline void pushVector4(lua_State *L, Tool::Vec4f v)
{
	lua_pushnumber(L, v.x);
	lua_pushnumber(L, v.y);
	lua_pushnumber(L, v.z);
	lua_pushnumber(L, v.t);
}

inline Tool::Vec2f toVector2(lua_State *L, int idx)
{
	float x, y;
	x = luaL_checknumber(L, idx);
	y = luaL_checknumber(L, idx+1);
	return Tool::Vec2f(x,y);
}

inline Tool::Vec3f toVector(lua_State *L, int idx)
{
	float x, y, z;
	x = luaL_checknumber(L, idx);
	y = luaL_checknumber(L, idx+1);
	z = luaL_checknumber(L, idx+2);
	return Tool::Vec3f(x,y,z);
}

inline Tool::Vec4f toVector4(lua_State *L, int idx)
{
	float x, y, z, t;
	x = luaL_checknumber(L, idx);
	y = luaL_checknumber(L, idx+1);
	z = luaL_checknumber(L, idx+2);
	t = luaL_checknumber(L, idx+3);
	return Tool::Vec4f(x,y,z,t);
}

#include "luabridge.hpp"

namespace luabridge
{

/**
 /warning This parameter type can only be used as last parameter
 /todo	Switch to using lua tables instead
*/
template <>
struct tdstack <Tool::Vec2f>
{
	static void push (lua_State *L, Tool::Vec2f data)
	{
		pushVector2(L, data);
	}
	static Tool::Vec2f get (lua_State *L, int index)
	{
		return toVector2(L, index);
	}
};

/**
 /warning This parameter type can only be used as last parameter
 /todo	Switch to using lua tables instead
*/
template <>
struct tdstack <Tool::Vec3f>
{
	static void push (lua_State *L, Tool::Vec3f data)
	{
		pushVector(L, data);
	}
	static Tool::Vec3f get (lua_State *L, int index)
	{
		return toVector(L, index);
	}
};

/**
 /warning This parameter type can only be used as last parameter
 /todo	Switch to using lua tables instead
*/
template <>
struct tdstack <Tool::Vec4f>
{
	static void push (lua_State *L, Tool::Vec4f data)
	{
		pushVector4(L, data);
	}
	static Tool::Vec4f get (lua_State *L, int index)
	{
		return toVector4(L, index);
	}
};

template <>
struct tdstack <char>
{
	static void push (lua_State *L, char data)
	{
		lua_pushinteger(L, data);
	}
	static char get (lua_State *L, int index)
	{
		return luaL_checkint(L, index);
	}
};

}

#endif

