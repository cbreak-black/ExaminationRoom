/*
 *  textureproxy.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 28.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "textureproxy.h"

#include "surfaces/abstracttexture.h"
#include "surfaces/texture.h"
#include "surfaces/stereogram.h"
#include "surfaces/randomdotstereogram.h"
#include "surfaces/patternstereogram.h"

#include "luahelper.h"

#include <string>

using namespace std::tr1;

namespace Examination
{

const char * textureTypes[] =
{
	"Simple",
	"Stereogram",
	"RandomDot",
	"Pattern",
	0
};

const char * textureStyles[] =
{
	"convex",
	"concave",
	0
};

TextureProxy::TextureProxy(lua_State *L)
{
	int n = lua_gettop(L);
	int opt = luaL_checkoption(L, 1, 0, textureTypes);

	switch (opt)
	{
		case 0:
		{
			// Simple Texture
			const char * p1 = luaL_checkstring(L, 2); // First Path
			texture_ = shared_ptr<Texture>(new Texture(std::string(p1)));
			break;
		}
		case 1:
		{
			// Stereogram
			const char * p1 = luaL_checkstring(L, 2); // First Path
			const char * p2 = luaL_checkstring(L, 3); // Second Path
			shared_ptr<Texture> t1 = shared_ptr<Texture>(new Texture(p1));
			shared_ptr<Texture> t2 = shared_ptr<Texture>(new Texture(p2));
			texture_ = shared_ptr<Stereogram>(new Stereogram(t1, t2));
			break;
		}
		case 2:
		{
			// Random Dot Stereogram
			const char * p1 = luaL_checkstring(L, 2); // First Path
			shared_ptr<Texture> t1 = shared_ptr<Texture>(new Texture(std::string(p1)));
			texture_ = shared_ptr<RandomdotStereogram>(new RandomdotStereogram(t1));
			break;
		}
		case 3:
		{
			// Pattern Stereogram
			const char * p1 = luaL_checkstring(L, 2); // First Path
			const char * p2 = luaL_checkstring(L, 3); // Second Path
			shared_ptr<Texture> depth = shared_ptr<Texture>(new Texture(p1));
			shared_ptr<Texture> pattern = shared_ptr<Texture>(new Texture(p2));
			if (n == 3)
			{
				texture_ = shared_ptr<PatternStereogram>(new PatternStereogram(depth, pattern));
			}
			else // (n == 4)
			{
				const char * p3 = luaL_checkstring(L, 4); // Second Path
				shared_ptr<Texture> patternFG = shared_ptr<Texture>(new Texture(p3));
				texture_ = shared_ptr<PatternStereogram>(new PatternStereogram(depth, pattern, patternFG));
			}
			break;
		}
	}
	lua_pop(L, n);
}
	
TextureProxy::~TextureProxy()
{
}

shared_ptr<AbstractTexture> TextureProxy::texture()
{
	return texture_;
}

std::tr1::shared_ptr<Stereogram> TextureProxy::stereogram()
{
	return dynamic_pointer_cast<Stereogram, AbstractTexture>(texture_);
}

std::tr1::shared_ptr<RandomdotStereogram> TextureProxy::rds()
{
	return dynamic_pointer_cast<RandomdotStereogram, AbstractTexture>(texture_);
}

int TextureProxy::zoom(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	Tool::Vec2f z = texture()->zoom();
	lua_pushnumber(L, z.x);
	lua_pushnumber(L, z.y);
	return 2;
}

int TextureProxy::setZoom(lua_State *L)
{
	checkTop(L, 3);
	texture()->setZoom(lua_tonumber(L,2), lua_tonumber(L,3));
	lua_pop(L, 3);
	return 0;
}

int TextureProxy::offset(lua_State *L)
{
	if (stereogram())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		lua_pushnumber(L, stereogram()->offset());
		return 1;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

int TextureProxy::setOffset(lua_State *L)
{
	if (stereogram())
	{
		checkTop(L, 2);
		stereogram()->setOffset(lua_tonumber(L,2));
		lua_pop(L, 2);
		return 0;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

int TextureProxy::style(lua_State *L)
{
	if (stereogram())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		lua_pushstring(L, textureStyles[stereogram()->style()]);
		return 1;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

int TextureProxy::setStyle(lua_State *L)
{
	if (stereogram())
	{
		checkTop(L, 2);
		stereogram()->setStyle((Stereogram::Style)luaL_checkoption(L, 2, 0, textureStyles));
		lua_pop(L, 2);
		return 0;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

int TextureProxy::setMaxColor(lua_State *L)
{
	if (rds())
	{
		checkTop(L, 2);
		rds()->setMaxColor(lua_tonumber(L, 2));
		lua_pop(L, 2);
		return 0;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

int TextureProxy::setExclusiveColor(lua_State *L)
{
	if (rds())
	{
		checkTop(L, 2);
		rds()->setExclusiveColor(lua_tonumber(L, 2));
		lua_pop(L, 2);
		return 0;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

int TextureProxy::setColor(lua_State *L)
{
	if (rds())
	{
		checkTop(L, 5);
		rds()->setColor(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
		lua_pop(L, 5);
		return 0;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

int TextureProxy::resetColor(lua_State *L)
{
	if (rds())
	{
		checkTop(L, 1);
		rds()->resetColor();
		lua_pop(L, 1);
		return 0;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

const char TextureProxy::className[] = "Texture";
const Luna<TextureProxy>::RegType TextureProxy::Register[] =
{
	{ "zoom", &TextureProxy::zoom },
	{ "setZoom", &TextureProxy::setZoom },
	{ "offset", &TextureProxy::offset },
	{ "setOffset", &TextureProxy::setOffset },
	{ "style", &TextureProxy::style },
	{ "setStyle", &TextureProxy::setStyle },
	{ "setMaxColor", &TextureProxy::setMaxColor },
	{ "setExclusiveColor", &TextureProxy::setExclusiveColor },
	{ "setColor", &TextureProxy::setColor },
	{ "resetColor", &TextureProxy::resetColor },
	{ 0, 0 }
};
	
	
}
