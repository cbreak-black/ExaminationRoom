/*
 *  objectproxy.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "objectproxy.h"

#include "rectangle.h"
#include "pixelplane.h"
#include "text.h"

#include "textureproxy.h"

#include "luahelper.h"

using namespace std::tr1;

namespace Examination
{

const char * errNotRect = "this is not a rectangle";
const char * errNotPP = "this is not a pixelplane";

const char * objectTypes[] =
{
	"Rectangle",
	"Pixelplane",
	"Text",
	0
};

ObjectProxy::ObjectProxy(lua_State *L)
{
	checkTop(L, 1);
	
	int opt = luaL_checkoption(L, 1, 0, objectTypes);	
	switch (opt)
	{
	case 0:
		rectangle_ = shared_ptr<Rectangle>(new Rectangle());
		object_ = rectangle_;
		break;
	case 1:
		pixelplane_ = shared_ptr<Pixelplane>(new Pixelplane());
		object_ = pixelplane_;
		break;
	case 2:
		text_ = shared_ptr<Text>(new Text());
		object_ = text_;
		break;
	}
	lua_pop(L, 0);
}

ObjectProxy::~ObjectProxy()
{
}
	
int ObjectProxy::dirA(lua_State *L)
{
	if (!rectangle_)
	{
		lua_pushstring(L, errNotRect);
		lua_error(L);
	}
	checkTop(L, 1);

	Tool::Vector v = rectangle()->dirA();
	lua_pop(L, 1);

	pushVector(L, v);
	return 3;
}

int ObjectProxy::dirB(lua_State *L)
{
	if (!rectangle_)
	{
		lua_pushstring(L, errNotRect);
		lua_error(L);
	}
	checkTop(L, 1);
	
	Tool::Vector v = rectangle()->dirB();
	lua_pop(L, 1);
	
	pushVector(L, v);
	return 3;
}

int ObjectProxy::setDirA(lua_State *L)
{
	if (!rectangle_)
	{
		lua_pushstring(L, errNotRect);
		lua_error(L);
	}
	checkTop(L, 4);
	rectangle()->setDirA(toVector(L,2));
	lua_pop(L, 4);
	return 0;
}

int ObjectProxy::setDirB(lua_State *L)
{
	if (!rectangle_)
	{
		lua_pushstring(L, errNotRect);
		lua_error(L);
	}
	checkTop(L, 4);
	rectangle()->setDirB(toVector(L,2));
	lua_pop(L, 4);
	return 0;
}

int ObjectProxy::setSize(lua_State *L)
{
	if (!pixelplane_)
	{
		lua_pushstring(L, errNotPP);
		lua_error(L);
	}
	checkTop(L, 3);
	float w, h;
	w = luaL_checknumber(L, 2);
	h = luaL_checknumber(L, 3);
	pixelplane()->setSize(w, h);
	lua_pop(L, 4);
	return 0;
}

int ObjectProxy::position(lua_State *L)
{
	checkTop(L, 1);
	Tool::Point p = object()->position();
	lua_pop(L, 1);
	
	pushVector(L, p);
	return 3;
}

int ObjectProxy::setPosition(lua_State *L)
{
	checkTop(L, 4);	
	object()->setPosition(toVector(L,2));
	lua_pop(L, 4);
	return 0;
}

int ObjectProxy::setTexCoords(lua_State *L)
{
if (!rectangle_)
	{
		lua_pushstring(L, errNotRect);
		lua_error(L);
	}
	checkTop(L, 9);
	
	float llx = luaL_checknumber(L, 2);
	float lly = luaL_checknumber(L, 3);
	float ulx = luaL_checknumber(L, 4);
	float uly = luaL_checknumber(L, 5);
	float lrx = luaL_checknumber(L, 6);
	float lry = luaL_checknumber(L, 7);
	float urx = luaL_checknumber(L, 8);
	float ury = luaL_checknumber(L, 9);
	
	rectangle()->setTexCoords(llx,lly, ulx, uly,
							  lrx, lry, urx, ury);

	lua_pop(L, 9);
	return 0;
}

int ObjectProxy::setTexture(lua_State *L)
{
	checkTop(L, 2);

	luaL_argcheck(L, lua_istable(L, 2), 2, "Not a Texture");
	lua_pushnumber(L, 0);
	lua_gettable(L, 2);

	TextureProxy ** t = static_cast<TextureProxy**>(luaL_checkudata(L, -1, TextureProxy::className));
	object()->setTexture((*t)->texture());
	lua_pop(L, 3);

	return 0;
}

int ObjectProxy::setAutoResize(lua_State *L)
{
	checkTop(L, 2);
	object()->setAutoResize(lua_toboolean(L, 2));
	lua_pop(L, 2);
	
	return 0;
}

int ObjectProxy::text(lua_State *L)
{
	if (text())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		lua_pushstring(L, qPrintable(text()->text()));
		return 1;
	}

	lua_settop(L, 0);
	return 0;
}

int ObjectProxy::setText(lua_State *L)
{
	if (text())
	{
		checkTop(L, 2);
		text()->setText(lua_tostring(L, 2));
		lua_pop(L, 2);
		return 0;
	}

	lua_settop(L,0);
	return 0;
}

shared_ptr<Rectangle> ObjectProxy::rectangle()
{
	return rectangle_;
}

shared_ptr<Pixelplane> ObjectProxy::pixelplane()
{
	return pixelplane_;
}

shared_ptr<Text> ObjectProxy::text()
{
	return text_;
}

shared_ptr<Object> ObjectProxy::object()
{
	return object_;
}

const char ObjectProxy::className[] = "Object";
const Luna<ObjectProxy>::RegType ObjectProxy::Register[] =
{
{ "dirA", &ObjectProxy::dirA },
{ "dirB", &ObjectProxy::dirB },
{ "setDirA", &ObjectProxy::setDirA },
{ "setDirB", &ObjectProxy::setDirB },
{ "setSize", &ObjectProxy::setSize },
{ "text", &ObjectProxy::text },
{ "setText", &ObjectProxy::setText },
{ "position", &ObjectProxy::position },
{ "setPosition", &ObjectProxy::setPosition },
{ "setTexCoords", &ObjectProxy::setTexCoords },
{ "setTexture", &ObjectProxy::setTexture },
{ "setAutoResize", &ObjectProxy::setAutoResize },
{ 0, 0 }
};

}
