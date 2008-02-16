/*
 *  camera.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 29.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"

namespace Examination
{
	class GLWidget;

/**
Represents a stereoscopic camera pair. It can load itself into the OpenGL
projection matrix stack.
 \author Gerhard Roethlin
*/
class Camera
{
public:
	/**
	Creates a camera with initial values.
	*/
	Camera();
	
	/**
	Creates a camera at the given position with the given viewing direction.
	 \param pos	The position of the camera in world space.
	 \param dir	The direction of the camera in world space.
	*/
	Camera(Tool::Point pos, Tool::Vector dir);

public:
	/**
	Loads the camera for the side of the view into the OpenGL matrix stack.
	*/
	void loadMatrix(GLWidget * dest);

public:
	void setPosition(Tool::Point pos);
	void setDirection(Tool::Vector dir);
	void setSeperation(float s);
	void setFieldOfView(float fov);
	void setParalaxPlane(float dist);
	
public:
	Tool::Point position();
	Tool::Vector directon();
	float seperation();
	float fieldOfView();
	float paralaxPlane();

private:
	Tool::Point		pos_;
	Tool::Vector	dir_;
	float			sep_;
	float			fov_;
	float			ppd_;
};

}
#endif
