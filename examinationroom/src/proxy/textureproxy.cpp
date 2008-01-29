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
		int c = lua_tonumber(L, 1); // Number of channels
		const char * p1 = lua_tostring(L, 2); // First Path
		if (c == 1)
		{
			if (p1)
			{
				texture_ = shared_ptr<AbstractTexture>(new Texture(std::string(p1)));
			}
			else
			{
				lua_pushstring(L, errArgT);
				lua_error(L);
				return;
			}
		}
		if (c == 2)
		{
			if (n == 3)
			{
				const char * p2 = lua_tostring(L, 3); // Second Path
				if (p2)
				{
					shared_ptr<Texture> t1 = shared_ptr<Texture>(new Texture(p1));
					shared_ptr<Texture> t2 = shared_ptr<Texture>(new Texture(p2));
					texture_ = shared_ptr<AbstractTexture>(new Stereogram(t1, t2));
				}
				else
				{
					lua_pushstring(L, errArgT);
					lua_error(L);
					return;
				}				
			}
			else if (p1)
			{
				shared_ptr<Texture> t1 = shared_ptr<Texture>(new Texture(std::string(p1)));
				texture_ = shared_ptr<AbstractTexture>(new Stereogram(t1));
			}
			else
			{
				lua_pushstring(L, errArgT);
				lua_error(L);
				return;
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
