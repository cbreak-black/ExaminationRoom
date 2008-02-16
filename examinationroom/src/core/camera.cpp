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

#include "screenproject.h"
#include "platform_math.h"

using namespace Tool;

namespace Examination
{

const float nearFactor = 0.1f;
const float farFactor = 5.0f;

Camera::Camera()
{
	setPosition(Tool::Point(0, 0, -10));
	setDirection(Tool::Vector(0,0,-1));

	setSeperation(0.2);
	setFieldOfView(50);
	setParalaxPlane(10);

	sp_ = new ScreenProject();
}

Camera::Camera(Tool::Point pos, Tool::Vector dir)
{
	setPosition(pos);
	setDirection(dir);
	
	setSeperation(0.2);
	setFieldOfView(50);
	setParalaxPlane(10);

	sp_ = new ScreenProject();
}

Camera::~Camera()
{
	delete sp_;
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
	glTranslatef(-pos_.x, -pos_.y, -pos_.z);

	sp_->calculateMVP();
}

ScreenProject * Camera::screenProject()
{
	return sp_;
}

int Camera::unitScreenSize(float d)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float near, top;
	float fovTan = tanf((fov_/2)/180*M_PI);
	near = ppd_*nearFactor;
	top = fovTan*near;
	// heightInPix / heightInUnits
	return viewport[3]/(top*2) * near/d;
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

Tool::Vector Camera::direction()
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
