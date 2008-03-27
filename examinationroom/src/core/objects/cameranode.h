/*
 *  cameranode.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.26.08.
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
This class represents a transformable group of objects.
*/
class CameraNode : public Object, public Container
{
public: // Construction
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
	virtual void draw(GLWidget * dest) const;

public: // Object protocol
	virtual void setScene(Scene * s);
	virtual void setParent(Container * c);

protected: // Container helpers
	virtual Container * getParent();
	virtual Scene * getScene();

private:
	std::tr1::shared_ptr<Camera> cam_;
};

}

#endif
