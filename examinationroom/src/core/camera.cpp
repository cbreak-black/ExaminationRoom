/*
 *  camera.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 29.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "camera.h"

#include "glwidget.h"

namespace Examination
{

Camera::Camera()
{
	setPosition(Tool::Point(0, 0, -10));
	setDirection(Tool::Vector(0,0,-1));

	setSeperation(0.2);
	setFieldOfView(50);
}

Camera::Camera(Tool::Point pos, Tool::Vector dir)
{
	setPosition(pos);
	setDirection(dir);
	
	setSeperation(0.2);
	setFieldOfView(50);
}

void Camera::loadMatrix(GLWidget * dest)
{
	float offsetFrame, offsetCamera;
	
	if (dest->side() == GLWidget::left)
	{
		offsetFrame = -0.2*sep_;
		offsetCamera = sep_;
	}
	else
	{
		offsetFrame = 0.2*sep_;
		offsetCamera = -sep_;
	}
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	float aspect = (float)viewport[2]/viewport[3];
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	glTranslatef(offsetFrame, 0, 0);		// Frame offset
	gluPerspective(fov_, aspect, 2, 20);	// Camera
	glTranslatef(offsetCamera, 0, 0);		// Camera offset
	glTranslatef(pos_.x, pos_.y, pos_.z);	// Camera position
    glMatrixMode(GL_MODELVIEW);	
}

void Camera::setPosition(Tool::Point pos)
{
	pos_ = pos;
}

void Camera::setDirection(Tool::Vector dir)
{
	dir_ = dir;
}

void Camera::setSeperation(float s)
{
	sep_ = s;
}

void Camera::setFieldOfView(float fov)
{
	fov_ = fov;
}

Tool::Point Camera::position()
{
	return pos_;
}

Tool::Vector Camera::directon()
{
	return dir_;
}

float Camera::seperation()
{
	return sep_;
}

float Camera::fieldOfView()
{
	return fov_;
}

}
