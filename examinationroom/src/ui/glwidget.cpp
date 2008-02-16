/*
 *  glwidget.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include <QtGui>
#include <QtOpenGL>

#include "platform_math.h"

#include "glwidget.h"

#include "scene.h"
#include "camera.h"

namespace Examination
{

GLWidget::GLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
{
	scene_ = 0;
	side_ = left;
	style_ = sidebyside;
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(200, 150);
}

QSize GLWidget::sizeHint() const
{
    return QSize(800, 600);
}

Scene * GLWidget::scene()
{
	return scene_;
}

void GLWidget::setScene(Scene * s)
{
//	if (scene_ && scene_ != s)
//		delete scene_;
	// Weak reference
	scene_ = s;
}

GLWidget::Side GLWidget::side()
{
	return side_;
}

void GLWidget::setSide(Side s)
{
	side_ = s;
}

GLWidget::DrawStyle GLWidget::style()
{
	return style_;
}

void GLWidget::setStyle(DrawStyle s)
{
	style_ = s;
}

void GLWidget::initializeGL()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0, 0, 0, 0);

	// For Indexed B&W
	GLfloat map[] = { 0.0f, 1.0f};
	GLfloat mapA[] = { 1.0f, 1.0f};
	glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 2,map);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 2,map);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 2,map);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 2,mapA);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Scene
	if (scene_)
	{
		// Left
		QSize s = this->size();
		if (style_ == anaglyph)
		{
			setSide(left);
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			//glColorMask(1, 0, 0, 1);
		}
		else if (style_ == sidebyside)
		{
			setSide(left);
			glViewport(0,0, s.width()/2, s.height());
		}

		scene_->camera()->loadMatrix(this);
		// Center
		glBegin(GL_POINTS);
		glVertex3f(0, 0, 0);
		glEnd();

		scene_->drawScene(this);

		// Right
		if (style_ == anaglyph)
		{
			setSide(right);
			glClear(GL_DEPTH_BUFFER_BIT);
			glColor4f(0.0f, 1.0f, 1.0f, 0.5f);
			//glColorMask(0, 1, 1, 1);
			scene_->camera()->loadMatrix(this);
			// Center
			glBegin(GL_POINTS);
			glVertex3f(0, 0, 0);
			glEnd();
			scene_->drawScene(this);
			//glColorMask(1, 1, 1, 1);
		}
		else if (style_ == sidebyside)
		{
			setSide(right);
			glViewport(s.width()/2,0, s.width()/2, s.height());
			scene_->camera()->loadMatrix(this);
			// Center
			glColor3f(1.0f, 1.0f, 1.0f);
			glBegin(GL_POINTS);
			glVertex3f(0, 0, 0);
			glEnd();
			scene_->drawScene(this);
			glViewport(0,0, s.width(), s.height());
		}
	}
}

void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
}

}
