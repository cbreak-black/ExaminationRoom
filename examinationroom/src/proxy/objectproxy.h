/*
 *  objectproxy.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef OBJECTPROXY_H
#define OBJECTPROXY_H

#include "luna.h"
#include <memory>

namespace Examination
{
	class Rectangle;
	class Parallelepiped;
	class Pixelplane;
	class Text;
	class Mesh;
	class AffineTransformation;
	class CameraNode;
	class Container;
	class Object;

class ObjectProxy
{
public:
	ObjectProxy(lua_State *L);

public: // From LUA
	int dirA(lua_State *L);
	int dirB(lua_State *L);
	int dirC(lua_State *L);
	int setDirA(lua_State *L);
	int setDirB(lua_State *L);
	int setDirC(lua_State *L);

public: // Object
	int position(lua_State *L);
	int setPosition(lua_State *L);
	int color(lua_State *L);
	int setColor(lua_State *L);
	int wireframe(lua_State *L);
	int setWireframe(lua_State *L);

public: // Textures
	int setTexCoords(lua_State *L);
	int setTexture(lua_State *L);

public: // Pixelplane
	int setSize(lua_State *L);
	int setAutoResize(lua_State *L);
	int resizeToCurrent(lua_State *L);
	int resizeTo(lua_State *L);

public: // Text
	int text(lua_State *L);
	int setText(lua_State *L);

public: // Mesh
	int loadMesh(lua_State *L);
	int clearMesh(lua_State *L);

public: // AffineTransformation
	int loadIdentity(lua_State *L);
	int translate(lua_State *L);
	int rotate(lua_State *L);
	int scale(lua_State *L);

public: // Container
	int addObject(lua_State *L);
	int removeObject(lua_State *L);
	int clear(lua_State *L);

public: // Dynamic Casts
	std::tr1::shared_ptr<Rectangle> rectangle();
	std::tr1::shared_ptr<Parallelepiped> parallelepiped();
	std::tr1::shared_ptr<Pixelplane> pixelplane();
	std::tr1::shared_ptr<Text> text();
	std::tr1::shared_ptr<Mesh> mesh();
	std::tr1::shared_ptr<AffineTransformation> affineTransformation();
	std::tr1::shared_ptr<CameraNode> cameraNode();
	std::tr1::shared_ptr<Container> container();
	std::tr1::shared_ptr<Object> object();
	
private:
	std::tr1::shared_ptr<Object> object_;

public: // LUNA
	static const char className[];
	static const Luna<ObjectProxy>::RegType Register[];
};

}
#endif
