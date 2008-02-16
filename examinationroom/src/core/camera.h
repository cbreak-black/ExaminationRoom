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

namespace Tool
{
	class ScreenProject;
}

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

	/**
	Destroys the camera
	*/
	~Camera();

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
	Tool::Vector direction();
	float seperation();
	float fieldOfView();
	float paralaxPlane();

public:
	/**
	Calculates and returns the size on screen in pixel of one unit at a given distance.
	Note that not the distance of the object itself to the camera, but the distance of
	the plane the object lies on matters.
	 \param d	Distance of line to camera
	 \return size of a line of length 1 at distance d on screen in pixel
	*/
	int unitScreenSize(float d);

public:
	Tool::ScreenProject * screenProject();

private:
	Tool::Point		pos_;
	Tool::Vector	dir_;
	float			sep_;
	float			fov_;
	float			ppd_;
	Tool::ScreenProject *	sp_;
};

}
#endif
