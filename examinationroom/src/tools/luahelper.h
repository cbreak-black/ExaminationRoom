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

template <typename V>
inline void pushVector(lua_State *L, V v)
{
	const int n = V::dim;
	lua_createtable(L, n, 0);
	for (int i = 0; i < n; i++)
	{
		lua_pushnumber(L, i+1);
		lua_pushnumber(L, v[i]);
		lua_settable(L, -3);
	}
}

template <typename V>
inline V toVector(lua_State *L, int idx)
{
	const int n = V::dim;
	V v;
	luaL_checktype(L, idx, LUA_TTABLE);
	for (int i = 0; i < n; i++)
	{
		lua_pushnumber(L, i+1);
		lua_gettable(L, idx);
		v[i] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	return v;
}

//template void pushVector<Tool::Vec2f,2>(lua_State*, Tool::Vec2f);
//template void pushVector<Tool::Vec3f,3>(lua_State*, Tool::Vec3f);
//template void pushVector<Tool::Vec4f,4>(lua_State*, Tool::Vec4f);
//template Tool::Vec2f toVector<Tool::Vec2f,2>(lua_State*, int idx);
//template Tool::Vec3f toVector<Tool::Vec3f,3>(lua_State*, int idx);
//template Tool::Vec4f toVector<Tool::Vec4f,4>(lua_State*, int idx);

#include "luabridge.hpp"

namespace luabridge
{

template <>
struct tdstack <Tool::Vec2f>
{
	static void push (lua_State *L, Tool::Vec2f data)
	{
		pushVector<Tool::Vec2f>(L, data);
	}
	static Tool::Vec2f get (lua_State *L, int index)
	{
		return toVector<Tool::Vec2f>(L, index);
	}
};

template <>
struct tdstack <Tool::Vec3f>
{
	static void push (lua_State *L, Tool::Vec3f data)
	{
		pushVector<Tool::Vec3f>(L, data);
	}
	static Tool::Vec3f get (lua_State *L, int index)
	{
		return toVector<Tool::Vec3f>(L, index);
	}
};

template <>
struct tdstack <Tool::Vec4f>
{
	static void push (lua_State *L, Tool::Vec4f data)
	{
		pushVector<Tool::Vec4f>(L, data);
	}
	static Tool::Vec4f get (lua_State *L, int index)
	{
		return toVector<Tool::Vec4f>(L, index);
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

/**
This instanciation is used to bind C++ code with raw lua commands to lua.
*/
template <typename T>
struct method_proxy <int (T::*)(lua_State*), int>
{
	typedef T classtype;
	typedef typelist<lua_State*> params;
	typedef int (T::*methodtype)(lua_State*);
	static int f (lua_State *L)
	{
		classtype *obj = ((shared_ptr<classtype> *)checkclass(L, 1,
															  lua_tostring(L, lua_upvalueindex(1))))->get();
		lua_pop(L, 2); // checkclass seems to push two meta tables on top of the stack
		methodtype fp = *(methodtype *)lua_touserdata(L, lua_upvalueindex(2));
		return (obj->*fp)(L);
	}
};

}

#endif

