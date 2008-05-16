/*
 *  cameranode.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 03.26.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

#include "container.h"

#include <memory>

namespace Examination
{
	class Camera;

/**
This class represents a group of objects that are viewn with it's own camera.
The result is an image which consists of objects seen through different cameras,
with potentialy different parameters.
 \see Camera
 \author Gerhard Roethlin
 \ingroup Objects
*/
class CameraNode : public Container
{
public: // Construction
	/**
	Constructs a default camera node with a default camera.
	Default draw priority is 128.
	 \warning Don't create instances of this class directly, use Object::Create()
	 \see Object::Create()
	*/
	CameraNode();

public:
	/**
	Returns a shared_ptr to the associated camera.
	 \return The camera that is associated with this node
	*/
	std::tr1::shared_ptr<Camera> camera() const;

	/**
	Sets the camera of this node to the given camera.
	 \param camera	The new camera
	*/
	void setCamera(std::tr1::shared_ptr<Camera> camera);

public:
	/**
	Draws the contained objects with the local camera.
	The depth buffer reset for this to work consistently.
	*/
	virtual void draw(GLWidget * dest) const;

public: // Serialisation
	/**
	Returns the name of the class of this object. This can be used in LUA
	object creation.
	 \return The name of this object's class as c++ string
	*/
	virtual std::string className() const;

	/**
	Writes the LUA commands to set parameters of this object to the output stream.
	 \param outStream	A stream that accepts writing
	*/
	virtual std::string toLua(std::ostream & outStream) const;

public: // LUA API
	static void registerLuaApi(luabridge::module * m);

private:
	std::tr1::shared_ptr<Camera> cam_;

public: // Meta
	static const char * className_;
};

}

#endif
