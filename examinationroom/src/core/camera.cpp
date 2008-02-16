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

#include "platform_math.h"

namespace Examination
{

Camera::Camera()
{
	setPosition(Tool::Point(0, 0, -10));
	setDirection(Tool::Vector(0,0,-1));

	setSeperation(0.2);
	setFieldOfView(50);
	setParalaxPlane(10);
}

Camera::Camera(Tool::Point pos, Tool::Vector dir)
{
	setPosition(pos);
	setDirection(dir);
	
	setSeperation(0.2);
	setFieldOfView(50);
	setParalaxPlane(10);
}

void Camera::loadMatrix(GLWidget * dest)
{
	// http://local.wasp.uwa.edu.au/~pbourke/projection/stereorender/
	float offsetCamera;
	
	if (dest->side() == GLWidget::left)
	{
		offsetCamera = sep_/2;
	}
	else
	{
		offsetCamera = -sep_/2;
	}
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	const float nearFactor = 0.1f;
	const float farFactor = 5.0f;
	float aspect = (float)viewport[2]/viewport[3];
	float top, bottom, left, right, near, far;
	float fovTan = tanf((fov_/2)/180*M_PI);
	near = ppd_*nearFactor;
	far = ppd_*farFactor;
	top = fovTan*near;
	bottom = -top;
	left = bottom*aspect;
	right = top*aspect;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left+offsetCamera, right+offsetCamera, bottom, top, near, far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// TODO: Rotation of camera before adjusting eye position
	glTranslatef(offsetCamera/nearFactor, 0, 0);
	glTranslatef(pos_.x, pos_.y, pos_.z);
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

void Camera::setParalaxPlane(float dist)
{
	ppd_ = dist;
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

float Camera::paralaxPlane()
{
	return ppd_;
}

}
