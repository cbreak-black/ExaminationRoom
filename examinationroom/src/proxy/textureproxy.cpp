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

TextureProxy::TextureProxy(lua_State *L)
{
	int n = lua_gettop(L);
	if (n >= 2)
	{
		int c = luaL_checknumber(L, 1); // Number of channels
		luaL_argcheck(L, (c == 1 || c == 2 || c == 3), 1, "Only one, two or three channels allowed");
		const char * p1 = luaL_checkstring(L, 2); // First Path
		if (c == 1)
		{
			texture_ = shared_ptr<AbstractTexture>(new Texture(std::string(p1)));
		}
		else if (c == 2)
		{
			if (n == 3)
			{
				const char * p2 = luaL_checkstring(L, 3); // Second Path
				shared_ptr<Texture> t1 = shared_ptr<Texture>(new Texture(p1));
				shared_ptr<Texture> t2 = shared_ptr<Texture>(new Texture(p2));
				texture_ = shared_ptr<AbstractTexture>(new Stereogram(t1, t2));
			}
			else
			{
				shared_ptr<Texture> t1 = shared_ptr<Texture>(new Texture(std::string(p1)));
				texture_ = shared_ptr<AbstractTexture>(new RandomdotStereogram(t1));
			}
		}
		else if (c == 3)
		{
			if (n == 3)
			{
				const char * p2 = luaL_checkstring(L, 3); // Second Path
				shared_ptr<Texture> depth = shared_ptr<Texture>(new Texture(p1));
				shared_ptr<Texture> pattern = shared_ptr<Texture>(new Texture(p2));
				texture_ = shared_ptr<AbstractTexture>(new PatternStereogram(depth, pattern));
			}
		}
	}
	else
	{
		lua_pushstring(L, errArgN);
		lua_error(L);
		return;
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

const char TextureProxy::className[] = "Texture";
const Luna<TextureProxy>::RegType TextureProxy::Register[] =
{
	{ 0, 0 }
};
	
	
}
