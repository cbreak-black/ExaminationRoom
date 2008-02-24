/*
 *  textureproxy.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 28.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "textureproxy.h"

#include "abstracttexture.h"
#include "texture.h"
#include "stereogram.h"
#include "randomdotstereogram.h"
#include "patternstereogram.h"

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
			stereogram_ = shared_ptr<Stereogram>(new Stereogram(t1, t2));
			texture_ = stereogram_;
			break;
		}
		case 2:
		{
			// Random Dot Stereogram
			const char * p1 = luaL_checkstring(L, 2); // First Path
			shared_ptr<Texture> t1 = shared_ptr<Texture>(new Texture(std::string(p1)));
			rds_ = shared_ptr<RandomdotStereogram>(new RandomdotStereogram(t1));
			stereogram_ = rds_;
			texture_ = stereogram_;
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
				stereogram_ = shared_ptr<PatternStereogram>(new PatternStereogram(depth, pattern));
			}
			else // (n == 4)
			{
				const char * p3 = luaL_checkstring(L, 4); // Second Path
				shared_ptr<Texture> patternFG = shared_ptr<Texture>(new Texture(p3));
				stereogram_ = shared_ptr<PatternStereogram>(new PatternStereogram(depth, pattern, patternFG));
			}
			texture_ = stereogram_;
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

int TextureProxy::zoom(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	lua_pushnumber(L, texture()->zoom());
	return 1;
}

int TextureProxy::setZoom(lua_State *L)
{
	checkTop(L, 2);
	texture()->setZoom(lua_tonumber(L,2));
	lua_pop(L, 2);
	return 0;
}

int TextureProxy::offset(lua_State *L)
{
	if (stereogram_)
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		lua_pushnumber(L, stereogram_->offset());
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
	if (stereogram_)
	{
		checkTop(L, 2);
		stereogram_->setOffset(lua_tonumber(L,2));
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
	if (stereogram_)
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		lua_pushstring(L, textureStyles[stereogram_->style()]);
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
	if (stereogram_)
	{
		checkTop(L, 2);
		stereogram_->setStyle((Stereogram::Style)luaL_checkoption(L, 2, 0, textureStyles));
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
	if (rds_)
	{
		checkTop(L, 2);
		rds_->setMaxColor(lua_tonumber(L, 2));
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
	if (rds_)
	{
		checkTop(L, 2);
		rds_->setExclusiveColor(lua_tonumber(L, 2));
		lua_pop(L, 2);
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
	{ 0, 0 }
};
	
	
}
