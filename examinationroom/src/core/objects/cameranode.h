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

#include "object.h"
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
*/
class CameraNode : public Container
{
public: // Construction
	/**
	Constructs a default camera node with a default camera.
	Default draw priority is 128.
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

private:
	std::tr1::shared_ptr<Camera> cam_;
};

}

#endif
