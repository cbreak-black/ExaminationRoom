/*
 *  textureproxy.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 28.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef TEXTUREPROXY_H
#define TEXTUREPROXY_H

#include "luna.h"
#include <memory>

namespace Examination
{
	class AbstractTexture;
	class Stereogram;
	class RandomdotStereogram;
	class PatternStereogram;

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
	int zoom(lua_State *L);
	int setZoom(lua_State *L);

public: // Stereogram
	int offset(lua_State *L);
	int setOffset(lua_State *L);
	int style(lua_State *L);
	int setStyle(lua_State *L);

public: // RDS
	int setMaxColor(lua_State *L);
	int setExclusiveColor(lua_State *L);

public:
	std::tr1::shared_ptr<AbstractTexture> texture();

private:
	std::tr1::shared_ptr<AbstractTexture> texture_;
	std::tr1::shared_ptr<Stereogram> stereogram_;
	std::tr1::shared_ptr<RandomdotStereogram> rds_;

public: // LUNA
	static const char className[];
	static const Luna<TextureProxy>::RegType Register[];
};

}
#endif
