/*
 *  objectproxy.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "objectproxy.h"

#include "objects/sphere.h"
#include "objects/rectangle.h"
#include "objects/parallelepiped.h"
#include "objects/pixelplane.h"
#include "objects/text.h"
#include "objects/mesh.h"
#include "objects/affinetransformation.h"
#include "objects/cameranode.h"
#include "objects/lightnode.h"

#include "textureproxy.h"
#include "cameraproxy.h"

#include "luahelper.h"

using namespace std::tr1;

namespace Examination
{

const char * objectTypes[] =
{
	"Sphere",
	"Rectangle",
	"Parallelepiped",
	"Pixelplane",
	"Text",
	"Mesh",
	"AffineTransformation",
	"CameraNode",
	"LightNode",
	0
};

ObjectProxy::ObjectProxy(lua_State *L)
{
	checkTop(L, 1);
	
	int opt = luaL_checkoption(L, 1, 0, objectTypes);	
	switch (opt)
	{
	case 0:
		object_ = shared_ptr<Object>(new Sphere());
		break;
	case 1:
		object_ = shared_ptr<Object>(new Rectangle());
		break;
	case 2:
		object_ = shared_ptr<Object>(new Parallelepiped());
		break;
	case 3:
		object_ = shared_ptr<Object>(new Pixelplane());
		break;
	case 4:
		object_ = shared_ptr<Object>(new Text());
		break;
	case 5:
		object_ = shared_ptr<Object>(new Mesh());
		break;
	case 6:
		object_ = shared_ptr<Object>(new AffineTransformation());
		break;
	case 7:
		object_ = shared_ptr<Object>(new CameraNode());
		break;
	case 8:
		object_ = shared_ptr<Object>(new LightNode());
		break;
	}
	lua_pop(L, 0);
}

// Sphere
int ObjectProxy::radius(lua_State *L)
{
	if (sphere())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		lua_pushnumber(L, sphere()->radius());
		return 1;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::setRadius(lua_State *L)
{
	if (sphere())
	{
		checkTop(L, 2);
		sphere()->setRadius(lua_tonumber(L, 2));
		lua_pop(L, 2);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}
int ObjectProxy::slices(lua_State *L)
{
	if (sphere())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		lua_pushnumber(L, sphere()->slices());
		return 1;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::setSlices(lua_State *L)
{
	if (sphere())
	{
		checkTop(L, 2);
		sphere()->setSlices(lua_tonumber(L, 2));
		lua_pop(L, 2);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::stacks(lua_State *L)
{
	if (sphere())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		lua_pushnumber(L, sphere()->stacks());
		return 1;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::setStacks(lua_State *L)
{
	if (sphere())
	{
		checkTop(L, 2);
		sphere()->setStacks(lua_tonumber(L, 2));
		lua_pop(L, 2);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

// Rectangle
int ObjectProxy::dirA(lua_State *L)
{
	if (rectangle())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		pushVector(L, rectangle()->dirA());
		return 3;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::dirB(lua_State *L)
{
	if (rectangle())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		pushVector(L, rectangle()->dirB());
		return 3;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::dirC(lua_State *L)
{
	if (parallelepiped())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		pushVector(L, parallelepiped()->dirC());
		return 3;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::setDirA(lua_State *L)
{
	if (rectangle())
	{
		checkTop(L, 4);
		rectangle()->setDirA(toVector(L,2));
		lua_pop(L, 4);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::setDirB(lua_State *L)
{
	if (rectangle())
	{
		checkTop(L, 4);
		rectangle()->setDirB(toVector(L,2));
		lua_pop(L, 4);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::setDirC(lua_State *L)
{
	if (parallelepiped())
	{
		checkTop(L, 4);
		parallelepiped()->setDirC(toVector(L,2));
		lua_pop(L, 4);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

// Pixelplane
int ObjectProxy::setSize(lua_State *L)
{
	if (pixelplane())
	{
		checkTop(L, 3);
		float w, h;
		w = luaL_checknumber(L, 2);
		h = luaL_checknumber(L, 3);
		pixelplane()->setSize(w, h);
		lua_pop(L, 4);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

// General Objects
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

int ObjectProxy::color(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	pushVector4(L, object()->color());
	return 4;
}

int ObjectProxy::setColor(lua_State *L)
{
	checkTop(L, 5);
	object()->setColor(toVector4(L,2));
	lua_pop(L, 5);
	return 0;
}

int ObjectProxy::wireframe(lua_State *L)
{
	checkTop(L, 1);
	lua_pop(L, 1);
	lua_pushboolean(L, object()->wireframe());
	return 1;
}

int ObjectProxy::setWireframe(lua_State *L)
{
	checkTop(L, 2);
	object()->setWireframe(lua_toboolean(L, 2));
	lua_pop(L, 2);
	return 0;
}

int ObjectProxy::setTexCoords(lua_State *L)
{
	if (rectangle())
	{
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
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::setTexture(lua_State *L)
{
	checkTop(L, 2);
	TextureProxy * t = Luna<TextureProxy>::extract(L, 2);
	object()->setTexture(t->texture());
	lua_pop(L, 2);
	return 0;
}

// Pixelplane
int ObjectProxy::setAutoResize(lua_State *L)
{
	if (pixelplane())
	{
		checkTop(L, 2);
		pixelplane()->setAutoResize(lua_toboolean(L, 2));
		lua_pop(L, 2);
		return 0;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

int ObjectProxy::resizeToCurrent(lua_State *L)
{
	if (pixelplane())
	{
		checkTop(L, 1);
		pixelplane()->resizeToCurrent();
		lua_pop(L, 1);
		return 0;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

int ObjectProxy::resizeTo(lua_State *L)
{
	if (pixelplane())
	{
		checkTop(L, 3);
		pixelplane()->resizeTo(lua_tonumber(L, 2), lua_tonumber(L, 3));
		lua_pop(L, 3);
		return 0;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
}

// Text Node
int ObjectProxy::text(lua_State *L)
{
	if (text())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		lua_pushstring(L, qPrintable(text()->text()));
		return 1;
	}
	else
	{
		lua_settop(L, 0);
		return 0;
	}
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
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

// Mesh Node
int ObjectProxy::loadMesh(lua_State *L)
{
	if (mesh())
	{
		checkTop(L, 2);
		bool success = mesh()->loadMesh(lua_tostring(L, 2));
		lua_pop(L, 2);
		lua_pushboolean(L, success);
		return 1;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::clearMesh(lua_State *L)
{
	if (mesh())
	{
		checkTop(L, 1);
		mesh()->clearMesh();
		lua_pop(L, 1);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

// AffineTransformation Node
int ObjectProxy::loadIdentity(lua_State *L)
{
	if (affineTransformation())
	{
		checkTop(L, 1);
		affineTransformation()->loadIdentity();
		lua_pop(L, 1);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::translate(lua_State *L)
{
	if (affineTransformation())
	{
		checkTop(L, 4);
		affineTransformation()->translate(toVector(L, 2));
		lua_pop(L, 4);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::rotate(lua_State *L)
{
	if (affineTransformation())
	{
		checkTop(L, 5);
		affineTransformation()->rotate(toVector(L, 2), lua_tonumber(L, 5));
		lua_pop(L, 5);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::scale(lua_State *L)
{
	if (affineTransformation())
	{
		checkTop(L, 4);
		affineTransformation()->scale(toVector(L, 2));
		lua_pop(L, 4);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

// Camera Node
int ObjectProxy::camera(lua_State *L)
{
	if (cameraNode())
	{
		checkTop(L, 1);
		lua_pop(L, 1);
		CameraProxy * cp = new CameraProxy(cameraNode()->camera());
		Luna<CameraProxy>::inject(L, cp);
		return 1;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::setCamera(lua_State *L)
{
	if (cameraNode())
	{
		checkTop(L, 2);
		CameraProxy * cp = Luna<CameraProxy>::extract(L, 2);
		cameraNode()->setCamera(cp->camera());
		lua_pop(L, 2);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

// Light Node

// Container Node
int ObjectProxy::addObject(lua_State *L)
{
	if (container())
	{
		checkTop(L, 2);
		ObjectProxy * o = Luna<ObjectProxy>::extract(L, 2);
		container()->addObject(o->object());
		lua_pop(L, 2);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::removeObject(lua_State *L)
{
	if (container())
	{
		checkTop(L, 2);
		luaL_argcheck(L, lua_istable(L, 2), 2, "Not an Object");
		lua_pushnumber(L, 0);
		lua_gettable(L, -2);
		ObjectProxy ** r = static_cast<ObjectProxy**>(luaL_checkudata(L, -1, ObjectProxy::className));
		container()->removeObject((*r)->object());
		lua_pop(L, 3);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

int ObjectProxy::clear(lua_State *L)
{
	if (container())
	{
		checkTop(L, 1);
		container()->clear();
		lua_pop(L, 1);
		return 0;
	}
	else
	{
		lua_settop(L,0);
		return 0;
	}
}

// Dynamic casts
shared_ptr<Sphere> ObjectProxy::sphere()
{
	return dynamic_pointer_cast<Sphere, Object>(object_);
}

shared_ptr<Rectangle> ObjectProxy::rectangle()
{
	return dynamic_pointer_cast<Rectangle, Object>(object_);
}

std::tr1::shared_ptr<Parallelepiped> ObjectProxy::parallelepiped()
{
	return dynamic_pointer_cast<Parallelepiped, Object>(object_);
}

shared_ptr<Pixelplane> ObjectProxy::pixelplane()
{
	return dynamic_pointer_cast<Pixelplane, Object>(object_);
}

shared_ptr<Text> ObjectProxy::text()
{
	return dynamic_pointer_cast<Text, Object>(object_);
}

shared_ptr<Mesh> ObjectProxy::mesh()
{
	return dynamic_pointer_cast<Mesh, Object>(object_);
}

shared_ptr<AffineTransformation> ObjectProxy::affineTransformation()
{
	return dynamic_pointer_cast<AffineTransformation, Object>(object_);
}

shared_ptr<CameraNode> ObjectProxy::cameraNode()
{
	return dynamic_pointer_cast<CameraNode, Object>(object_);
}

shared_ptr<LightNode> ObjectProxy::lightNode()
{
	return dynamic_pointer_cast<LightNode, Object>(object_);
}

shared_ptr<Container> ObjectProxy::container()
{
	return dynamic_pointer_cast<Container, Object>(object_);
}

shared_ptr<Object> ObjectProxy::object()
{
	return object_;
}

const char ObjectProxy::className[] = "Object";
const Luna<ObjectProxy>::RegType ObjectProxy::Register[] =
{
	{ "radius", &ObjectProxy::radius },
	{ "setRadius", &ObjectProxy::setRadius },
	{ "slices", &ObjectProxy::slices },
	{ "setSlices", &ObjectProxy::setSlices },
	{ "stacks", &ObjectProxy::stacks },
	{ "setStacks", &ObjectProxy::setStacks },
	{ "dirA", &ObjectProxy::dirA },
	{ "dirB", &ObjectProxy::dirB },
	{ "dirC", &ObjectProxy::dirC },
	{ "setDirA", &ObjectProxy::setDirA },
	{ "setDirB", &ObjectProxy::setDirB },
	{ "setDirC", &ObjectProxy::setDirC },
	{ "setSize", &ObjectProxy::setSize },
	{ "text", &ObjectProxy::text },
	{ "setText", &ObjectProxy::setText },
	{ "loadMesh", &ObjectProxy::loadMesh },
	{ "clearMesh", &ObjectProxy::clearMesh },
	{ "loadIdentity", &ObjectProxy::loadIdentity },
	{ "translate", &ObjectProxy::translate },
	{ "rotate", &ObjectProxy::rotate },
	{ "scale", &ObjectProxy::scale },
	{ "camera", &ObjectProxy::camera },
	{ "setCamera", &ObjectProxy::setCamera },
	{ "addObject", &ObjectProxy::addObject },
	{ "removeObject", &ObjectProxy::removeObject },
	{ "clear", &ObjectProxy::clear },
	{ "position", &ObjectProxy::position },
	{ "setPosition", &ObjectProxy::setPosition },
	{ "color", &ObjectProxy::color },
	{ "setColor", &ObjectProxy::setColor },
	{ "wireframe", &ObjectProxy::wireframe },
	{ "setWireframe", &ObjectProxy::setWireframe },
	{ "setTexCoords", &ObjectProxy::setTexCoords },
	{ "setTexture", &ObjectProxy::setTexture },
	{ "setAutoResize", &ObjectProxy::setAutoResize },
	{ "resizeToCurrent", &ObjectProxy::resizeToCurrent },
	{ "resizeTo", &ObjectProxy::resizeTo },
	{ 0, 0 }
};

}
