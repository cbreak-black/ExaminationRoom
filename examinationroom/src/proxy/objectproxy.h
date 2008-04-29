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
	class Sphere;
	class Rectangle;
	class Parallelepiped;
	class Pixelplane;
	class Text;
	class Mesh;
	class AffineTransformation;
	class CameraNode;
	class LightNode;
	class Atmosphere;
	class DepthBuffer;
	class Container;
	class Object;

/**
This class binds Objects from C++ to the LUA world.
Most functions here are a 1 to 1 mapping from C++ to LUA, only when C++ Vec objects
are required, the individual numbers are used. Some accessors were simplified since
LUA is able to return multiple values.
 \ingroup LUA
*/
class ObjectProxy
{
public:
	/**
	The constructor of the ObjectProxy. It expects a valid Class Name as argument.
	If a valid name is passed, the class is instanciated and associated with the new
	proxy instance.
	If the name is not valid, bad things such as memory leaks might happen.
	 \param L	The LUA stack containing the argument
	 \see objectTypes
	*/
	ObjectProxy(lua_State *L);

public: // Sphere
	int radius(lua_State *L);
	int setRadius(lua_State *L);
	int slices(lua_State *L);
	int setSlices(lua_State *L);
	int stacks(lua_State *L);
	int setStacks(lua_State *L);

public: // Rectangle/Parallelepiped
	int dirA(lua_State *L);
	int dirB(lua_State *L);
	int dirC(lua_State *L);
	int setDirA(lua_State *L);
	int setDirB(lua_State *L);
	int setDirC(lua_State *L);
	int subdivision(lua_State *L);
	int setSubdivision(lua_State *L);

public: // Object
	int name(lua_State *L);
	int setName(lua_State *L);
	int position(lua_State *L);
	int setPosition(lua_State *L);
	int color(lua_State *L);
	int setColor(lua_State *L);
	int wireframe(lua_State *L);
	int setWireframe(lua_State *L);
	int drawPriority(lua_State *L);
	int setDrawPriority(lua_State *L);
	int shown(lua_State *L);
	int setShown(lua_State *L);
	int visible(lua_State *L);

public: // Textures
	int setTexCoords(lua_State *L);
	int setTexture(lua_State *L);

public: // Pixelplane
	int size(lua_State *L);
	int setSize(lua_State *L);
	int autoResize(lua_State *L);
	int setAutoResize(lua_State *L);
	int resizeToCurrent(lua_State *L);
	int resizeTo(lua_State *L);
	int zoom(lua_State *L);
	int setZoom(lua_State *L);

public: // Text
	int text(lua_State *L);
	int setText(lua_State *L);

public: // Mesh
	int loadMesh(lua_State *L);
	int clearMesh(lua_State *L);
	int scaleFactor(lua_State *L);
	int setScaleFactor(lua_State *L);

public: // AffineTransformation
	int loadIdentity(lua_State *L);
	int translate(lua_State *L);
	int rotate(lua_State *L);
	int scale(lua_State *L);

public: // Camera Node
	int camera(lua_State *L);
	int setCamera(lua_State *L);

public: // Light Node
	int ambient(lua_State *L);
	int setAmbient(lua_State *L);

public: // Atmosphere Node
	int mode(lua_State *L);
	int setMode(lua_State *L);
	int density(lua_State *L);
	int setDensity(lua_State *L);
	int start(lua_State *L);
	int setStart(lua_State *L);
	int end(lua_State *L);
	int setEnd(lua_State *L);

public: // DepthBuffer Node
	int depthBufferState(lua_State *L);
	int setDepthBufferState(lua_State *L);

public: // Container
	int addObject(lua_State *L);
	int removeObject(lua_State *L);
	int clear(lua_State *L);
	int enabled(lua_State *L);
	int setEnabled(lua_State *L);

public: // Dynamic Casts
	/**
	Returns the associated object as Sphere,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<Sphere> sphere();

	/**
	Returns the associated object as Rectangle,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<Rectangle> rectangle();

	/**
	Returns the associated object as Parallelepiped,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<Parallelepiped> parallelepiped();

	/**
	Returns the associated object as Pixelplane,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<Pixelplane> pixelplane();

	/**
	Returns the associated object as Text,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<Text> text();

	/**
	Returns the associated object as Mesh,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<Mesh> mesh();

	/**
	Returns the associated object as AffineTransformation,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<AffineTransformation> affineTransformation();

	/**
	Returns the associated object as CameraNode,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<CameraNode> cameraNode();

	/**
	Returns the associated object as LightNode,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<LightNode> lightNode();

	/**
	Returns the associated object as Atmosphere,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<Atmosphere> atmosphere();

	/**
	Returns the associated object as DepthBuffer,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<DepthBuffer> depthBuffer();

	/**
	Returns the associated object as Container,
	or 0 if it can not be dynamically cast.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<Container> container();

	/**
	Returns the associated object as Object.
	 \return	A shared_ptr to the associated object
	*/
	std::tr1::shared_ptr<Object> object();
	
private:
	std::tr1::shared_ptr<Object> object_;

public: // LUNA
	static const char className[];
	static const Luna<ObjectProxy>::RegType Register[];

	/**
	Null terminated list of valid object names.
	*/
	static const char * objectTypes[];
};

}
#endif
