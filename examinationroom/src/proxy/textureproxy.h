/*
 *  textureproxy.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 28.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TEXTUREPROXY_H
#define TEXTUREPROXY_H

#include "luna.h"
#include <memory>

namespace Examination
{
	class AbstractTexture;

/**
A proxy class to interface LUA and C++ AbsstractTexture objects.
 \author Gerhard Roethlin
*/
class TextureProxy
{
public:
	TextureProxy(lua_State *L);
	~TextureProxy();

public:
	std::tr1::shared_ptr<AbstractTexture> texture();

private:
	std::tr1::shared_ptr<AbstractTexture> texture_;

public: // LUNA
	static const char className[];
	static const Luna<TextureProxy>::RegType Register[];
};

}
#endif
