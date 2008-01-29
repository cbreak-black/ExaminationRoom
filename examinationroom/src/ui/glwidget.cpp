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
//	if (scene_ && scene_ != s)
//		delete scene_;
	// Weak reference
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

	// Left
	glColorMask(true, false, false, true);
	setSide(left);
	
	// Fixed camera, bad!
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glTranslatef(-0.04, 0, 0);		// Frame Adjustment
	gluPerspective(50, 4/3, 2, 20);	// Camera
	glTranslatef(+0.2, 0, -10);		// Camera position
    glMatrixMode(GL_MODELVIEW);	
	
	// Scene
	if (scene_)
		scene_->drawScene(this);

	// Right
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(false, true, true, true);
	setSide(right);

	// Fixed camera, bad!
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glTranslatef(+0.04, 0, 0);		// Frame Adjustment
	gluPerspective(50, 4/3, 2, 20);	// Camera
	glTranslatef(-0.2, 0, -10);		// Camera position
    glMatrixMode(GL_MODELVIEW);	
	
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
