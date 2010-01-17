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

#include "glerrortool.h"

using namespace Tool;

namespace Examination
{

const float nearFactor = 0.1f;
const float farFactor = 5.0f;

Camera::Camera()
{
	spL_ = new ScreenProject();
	spR_ = new ScreenProject();

	pos_ = Tool::Point(0, 0, 20);
	dir_ = Tool::Vector(0,0,-1);
	up_ = Tool::Vector(0,1,0);

	sep_ = 0.2f;
	fov_ = 25;
	ppd_ = 20;
	type_ = Perspective;
	preLoadMatrix();
}

Camera::Camera(Tool::Point pos, Tool::Vector dir, Tool::Vector up)
{
	spL_ = new ScreenProject();
	spR_ = new ScreenProject();

	pos_ = pos;
	dir_ = dir;
	up_ = up;

	sep_ = 0.2f;
	fov_ = 25;
	ppd_ = 20;
	type_ = Perspective;
	preLoadMatrix();
}

Camera::Camera(const Camera & c)
{
	spL_ = new ScreenProject();
	spR_ = new ScreenProject();

	pos_ = c.position();
	dir_ = c.direction();
	up_ = c.up();

	sep_ = c.separation();
	fov_ = c.fieldOfView();
	ppd_ = c.paralaxPlane();
	type_ = c.type();
	preLoadMatrix();
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
	GlErrorTool::getErrors("Camera::loadMatrix:1");
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float aspect = (float)viewport[2]/viewport[3];
	float fovTan = tanf((fov_/2)/180*M_PI);
	if (type() == Camera::Perspective)
	{
		// http://local.wasp.uwa.edu.au/~pbourke/projection/stereorender/

		float fTop, fBottom, fLeft, fRight, fNear, fFar;
		// Calculate fNear and fFar based on paralax plane distance hardcoded factors
		fNear = ppd_*nearFactor;
		fFar = ppd_*farFactor;
		// Calculate fTop and fBottom based on vertical field-of-view and distance
		fTop = fovTan*fNear;
		fBottom = -fTop;
		// Calculate fLeft and fRight basaed on aspect ratio
		fLeft = fBottom*aspect;
		fRight = fTop*aspect;

		glMatrixMode(GL_PROJECTION);
		// Projection matrix is a frustum, of which fLeft and fRight are not symetric
		// to set the zero paralax plane. The cameras are parallel.
		glPushMatrix();
		glLoadIdentity();
		glFrustum(fLeft+offsetCamera, fRight+offsetCamera, fBottom, fTop, fNear, fFar);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		// Rotation of camera and adjusting eye position
		Vector sepVec = cross(dir_, up_); // sepVec is normalized because dir and up are normalized
		sepVec *= offsetCamera/nearFactor;
		// Set camera position, direction and orientation
		gluLookAt(pos_.x - sepVec.x, pos_.y - sepVec.y, pos_.z - sepVec.z,
				  pos_.x - sepVec.x + dir_.x, pos_.y - sepVec.y + dir_.y, pos_.z - sepVec.z + dir_.z,
				  up_.x, up_.y, up_.z);
		GlErrorTool::getErrors("Camera::loadMatrix:2");
	}
	else if (type() == Camera::Parallel)
	{
		float fTop, fBottom, fLeft, fRight, fNear, fFar;
		// Calculate fNear and fFar based on paralax plane distance and a hardcoded factor
		// Note: the zero paralax plane is exactly in between near and far
		fFar = ppd_*farFactor;
		fNear = 2*ppd_ - fFar; // = ppd_ - (fFar-ppd_);
		// Set fTop and fBottom based on field-of-view and paralax plane distance
		// This is done to make the scaling of the image at the paralax plane the same
		// as in perspective mode
		fTop = fovTan*ppd_;
		fBottom = -fTop;
		// Set left and right baased on aspect ratio
		fLeft = fBottom*aspect;
		fRight = fTop*aspect;

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		// http://wireframe.doublemv.com/2006/08/11/projections-and-opengl/
		// Note: The code there is wrong, see below for correct code
		// Create oblique projection matrix by shearing an orthographic
		// Projection matrix. Those cameras are converged.
		const float shearMatrix[] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			-offsetCamera/nearFactor, 0, 1, 0,
			0, 0, 0, 1
		};
		glMultMatrixf(shearMatrix);
		glOrtho(fLeft, fRight, fBottom, fTop, fNear, fFar);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		// Rotation of camera
		// Note: The position of both left and right camera is at the same place
		//	because the offset is already calculated by the shearing, which also sets
		//	the zero paralax plane.
		gluLookAt(pos_.x, pos_.y, pos_.z,
				  pos_.x + dir_.x, pos_.y + dir_.y, pos_.z + dir_.z,
				  up_.x, up_.y, up_.z);
		GlErrorTool::getErrors("Camera::loadMatrix:3");
	}
	else if (type() == Camera::Screen)
	{
		// Ignore camera directin
		// Projection: Screen space coordinate system, 0 in center, width/height as
		// specified by viewport
		// Separation: One time camera separation per unit in z
		const float fRight = viewport[2]/2;
		const float fLeft = -fRight;
		const float fTop = viewport[3]/2;
		const float fBottom = -fTop;
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		const float shearMatrix[] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			-offsetCamera, 0, 1, 0,
			0, 0, 0, 1
		};
		glMultMatrixf(shearMatrix);
		glOrtho(fLeft,fRight,fBottom,fTop, fTop, fBottom);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		GlErrorTool::getErrors("Camera::loadMatrix:4");
	}
}

bool Camera::unloadMatrix()
{
	GlErrorTool::getErrors("Camera::unloadCamera:1");
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	GlErrorTool::getErrors("Camera::unloadCamera:2");
	return true;
}

void Camera::preLoadMatrix()
{
	GlErrorTool::getErrors("Camera::preLoadMatrix:1");
	// Hopefully an old context is still active
	// (Since contexts are never deactivated without making an other active,
	// it should work)
	loadMatrix(sep_/2);
	spL_->calculateMVP();
	unloadMatrix();
	loadMatrix(-sep_/2);
	spR_->calculateMVP();
	unloadMatrix();
	GlErrorTool::getErrors("Camera::preLoadMatrix:2");
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
	float fovTan = tanf((fov_/2)/180*M_PI);
	if (type() == Camera::Perspective)
	{
		float fNear, fTop;
		fNear = ppd_*nearFactor;
		fTop = fovTan*fNear;
		// heightInPix / heightInUnits
		return (float)viewport[3]/(fTop*2) * fNear/d;
	}
	else if (type() == Camera::Parallel)
	{
		float fTop = fovTan*ppd_;
		// heightInPix / heightInUnits
		return (float)viewport[3]/(fTop*2);
	}
	else // type() == Camera::Screen
	{
		return 1;
	}
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
	preLoadMatrix();
}

std::string Camera::toLua(std::ostream & outStream, const std::string & name) const
{
	outStream << name << ":" << "setType(\"";
	switch (type())
	{
		case Camera::Perspective:
			outStream << "Perspective";
			break;
		case Camera::Parallel:
			outStream << "Parallel";
			break;
		case Camera::Screen:
			outStream << "Screen";
			break;
	}
	outStream << "\");\n";
	outStream << name << ":" << "setPosition({"
		<< position().x << ", " << position().y << ", " << position().z << "});\n";
	outStream << name << ":" << "setDirection({"
		<< direction().x << ", " << direction().y << ", " << direction().z << "});\n";
	outStream << name << ":" << "setUp({"
		<< up().x << ", " << up().y << ", " << up().z << "});\n";
	outStream << name << ":" << "setSeparation(" << separation() << ");\n";
	outStream << name << ":" << "setFieldOfView(" << fieldOfView() << ");\n";
	outStream << name << ":" << "setParalaxPlane(" << paralaxPlane() << ");\n";
	return name;
}

}
