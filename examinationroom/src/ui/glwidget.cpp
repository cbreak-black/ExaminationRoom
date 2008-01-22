/*******************************************************************************

Examination Room is a flexible user evaluation tool

Copyright (C) 2008 Gerhard Roethlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************************/

#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"

#include "scene.h"

namespace Examination
{

GLWidget::GLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
{
	scene_ = 0;
	side_ = left;
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
	scene_ = s;
}

Side GLWidget::side()
{
	return side_;
}

void GLWidget::setSide(Side s)
{
	side_ = s;
}

void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_CULL_FACE);
	
	glClearColor(0, 0, 0, 0);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Center
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex3f(0, 0, 0);
	glEnd();
	
	// Scene
	if (scene_)
		scene_->drawScene(this);
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

	// Fixed camera, bad!
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
	gluPerspective(50, width/height, 2, 20);
	glTranslatef(0, 0, -10);
    glMatrixMode(GL_MODELVIEW);
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
