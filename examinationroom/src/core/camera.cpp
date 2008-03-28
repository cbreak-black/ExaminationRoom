/*
 *  camera.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 29.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "camera.h"

#include "screenproject.h"
#include "platform_math.h"
#include "float.h"

using namespace Tool;

namespace Examination
{

const float nearFactor = 0.1f;
const float farFactor = 5.0f;

Camera::Camera()
{
	spL_ = new ScreenProject();
	spR_ = new ScreenProject();

	setPosition(Tool::Point(0, 0, -10));
	setDirection(Tool::Vector(0,0,-1));
	setUp(Tool::Vector(0,1,0));

	setSeparation(0.2);
	setFieldOfView(50);
	setParalaxPlane(10);
	setType(Perspective);
}

Camera::Camera(Tool::Point pos, Tool::Vector dir, Tool::Vector up)
{
	spL_ = new ScreenProject();
	spR_ = new ScreenProject();

	setPosition(pos);
	setDirection(dir);
	setUp(up);
	
	setSeparation(0.2);
	setFieldOfView(50);
	setParalaxPlane(10);
	setType(Perspective);
}

Camera::~Camera()
{
	delete spL_;
	delete spR_;
}

void Camera::loadMatrix(GLWidget * dest)
{
	if (dest->side() == GLWidget::left)
	{
		loadMatrix(sep_/2);
	}
	else
	{
		loadMatrix(-sep_/2);
	}
}

void Camera::loadMatrix(float offsetCamera)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float aspect = (float)viewport[2]/viewport[3];
	float fovTan = tanf((fov_/2)/180*M_PI);
	if (type() == Camera::Perspective)
	{
		// http://local.wasp.uwa.edu.au/~pbourke/projection/stereorender/

		float top, bottom, left, right, near, far;
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
		// Rotation of camera and adjusting eye position
		Vector sepVec = cross(dir_, up_); // sepVec is normalized because dir and up are normalized
		sepVec *= offsetCamera/nearFactor;
		gluLookAt(pos_.x - sepVec.x, pos_.y - sepVec.y, pos_.z - sepVec.z,
				  pos_.x - sepVec.x + dir_.x, pos_.y - sepVec.y + dir_.y, pos_.z - sepVec.z + dir_.z,
				  up_.x, up_.y, up_.z);
	}
	else if (type() == Camera::Parallel)
	{
		float top, bottom, left, right, near, far;
		far = ppd_*farFactor;
		near = 2*ppd_ - far; // = ppd_ - (far-ppd_);
		top = fovTan*ppd_;
		bottom = -top;
		left = bottom*aspect;
		right = top*aspect;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// http://wireframe.doublemv.com/2006/08/11/projections-and-opengl/
		float theta = 90- atan(ppd_/offsetCamera*nearFactor);
		float shearMatrix[] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			-offsetCamera/nearFactor, 0, 1, 0,
			0, 0, 0, 1
		};
		glMultMatrixf(shearMatrix);
		glOrtho(left, right, bottom, top, near, far);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// Rotation of camera and adjusting eye position
		//Vector sepVec = cross(dir_, up_); // sepVec is normalized because dir and up are normalized
		//sepVec *= offsetCamera/nearFactor;
		gluLookAt(pos_.x, pos_.y, pos_.z,
				  pos_.x + dir_.x, pos_.y + dir_.y, pos_.z + dir_.z,
				  up_.x, up_.y, up_.z);
	}
}

void Camera::preLoadMatrix()
{
	// Hopefully an old context is still active
	// (Since contexts are never deactivated without making an other active,
	// it should work)
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	loadMatrix(sep_/2);
	spL_->calculateMVP();
	loadMatrix(-sep_/2);
	spR_->calculateMVP();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

ScreenProject * Camera::screenProject(GLWidget::Side s) const
{
	if (s == GLWidget::left)
		return spL_;
	else
		return spR_;
}

float Camera::unitScreenSize(float d) const
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float near, top;
	float fovTan = tanf((fov_/2)/180*M_PI);
	near = ppd_*nearFactor;
	top = fovTan*near;
	// heightInPix / heightInUnits
	return (float)viewport[3]/(top*2) * near/d;
}

float Camera::unitScreenSize(Point p) const
{
	float df = fabsf((position() - p) * direction());
	return unitScreenSize(df);
}

//float Camera::separationAtDistance(float d)
//{
//	if (d <= 0)
//	{
//		// Nothing should be behind the camera
//		return FLT_MAX;
//	}
//	else if (d < ppd_)
//	{
//		// Negative Paralax
//		return sep_ * (d - ppd_)/d;
//	}
//	else
//	{
//		// Positive Paralax (Same as above)
//		return sep_ * (d - ppd_)/d;
//	}
//}

float Camera::separationAtPoint(Point p) const
{
	Point pL = spL_->transformToScreenSpace(p);
	Point pR = spR_->transformToScreenSpace(p);
	return pR.x - pL.x;
}

void Camera::setPosition(Tool::Point pos)
{
	pos_ = pos;
	preLoadMatrix();
}

void Camera::setDirection(Tool::Vector dir)
{
	dir_ = dir;
	dir_.normalize();
	preLoadMatrix();
}

void Camera::setUp(Tool::Vector up)
{
	up_ = up;
	up_.normalize();
	preLoadMatrix();
}

void Camera::setSeparation(float s)
{
	sep_ = s;
	preLoadMatrix();
}

void Camera::setFieldOfView(float fov)
{
	fov_ = fov;
	preLoadMatrix();
}

void Camera::setParalaxPlane(float dist)
{
	ppd_ = dist;
	preLoadMatrix();
}

Tool::Point Camera::position() const
{
	return pos_;
}

Tool::Vector Camera::direction() const
{
	return dir_;
}

Tool::Vector Camera::up() const
{
	return up_;
}

float Camera::separation() const
{
	return sep_;
}

float Camera::fieldOfView() const
{
	return fov_;
}

float Camera::paralaxPlane() const
{
	return ppd_;
}

Camera::Type Camera::type() const
{
	return type_;
}

void Camera::setType(Camera::Type t)
{
	type_ = t;
}

}
