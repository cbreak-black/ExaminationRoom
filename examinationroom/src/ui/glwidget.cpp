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

#include <iostream>

#include "platform_math.h"

#include "glwidget.h"

#include "scene.h"
#include "camera.h"

namespace Examination
{

const Tool::Color4 black(0,0,0,1);

static const float colorMatrixLeft[16] = {
	1, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0.5, 0,
	0, 0, 0, 1
};
static const float colorMatrixRight[16] = {
	0, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 0.5, 0,
	0, 0, 0, 1
};

GLWidget::GLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
{
	side_ = left;
	style_ = sidebyside;
	this->setCursor(Qt::BlankCursor);
}

GLWidget::GLWidget(const QGLFormat & format, QWidget *parent, QGLWidget *shareWidget)
	: QGLWidget(format, parent, shareWidget)
{
	side_ = left;
	style_ = sidebyside;
	this->setCursor(Qt::BlankCursor);
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

std::tr1::shared_ptr<Scene> GLWidget::scene()
{
	return scene_;
}

void GLWidget::setScene(std::tr1::shared_ptr<Scene> s)
{
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
	// Some capability testing
	if (style_ == matrix)
	{
		GLint auxNum;
		glGetIntegerv(GL_AUX_BUFFERS, &auxNum);
		if (auxNum < 2)
		{
			std::cerr << "Not enough aux buffers support found. Matrix stereo will not work." << std::endl;
		}
	}
	else if (style_ == quad)
	{
		if (!format().stereo())
		{
			std::cerr << "No Stereo support found. Quad Buffer mode will not work" << std::endl;
		}
	}
}

void GLWidget::initializeGL()
{
	// Enable z-buffering
	glEnable(GL_DEPTH_TEST);

	// Stencil
	glClearStencil(0);
	glStencilFunc(GL_EQUAL, 0x01, 0x01);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

	// Set texture parameters
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Set blending parameters
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set global light settings
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black.vec);
	// and color material settings
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void GLWidget::paintGL()
{
	// Scene
	if (scene_)
	{
		Tool::Vec4f c = scene_->backgroundColor();
		glClearColor(c.r, c.g, c.b, c.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Left
		QSize s = this->size();
		if (style_ == anaglyph)
		{
			setSide(left);
			//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			glColorMask(1, 0, 0, 1);
		}
		else if (style_ == matrix)
		{
			setSide(left);
			glDrawBuffer(GL_AUX1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawBuffer(GL_AUX0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else if (style_ == sidebyside)
		{
			setSide(left);
			glViewport(0,0, s.width()/2, s.height());
		}
		else if (style_ == quad)
		{
			setSide(left);
			// Should be GL_BACK_LEFT from last pass
			//glDrawBuffer(GL_BACK_LEFT);
		}
		else if (style_ == line)
		{
			setSide(left);
			lineStencil();
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 0x01, 0x01);
		}

		scene_->camera()->loadMatrix(this);
		scene_->draw(this);

		// Right
		if (style_ == anaglyph)
		{
			setSide(right);
			glClear(GL_DEPTH_BUFFER_BIT);
			//glColor4f(0.0f, 1.0f, 1.0f, 0.5f);
			glColorMask(0, 1, 1, 1);
			scene_->camera()->loadMatrix(this);
			scene_->draw(this);
			glColorMask(1, 1, 1, 1);
		}
		else if (style_ == matrix)
		{
			// Prepare for drawing into the right aux buffer
			setSide(right);
			glDrawBuffer(GL_AUX1);
			// Draw
			scene_->camera()->loadMatrix(this);
			scene_->draw(this);
			// Write composite into the back buffer
			glDrawBuffer(GL_BACK);
			glDisable(GL_DEPTH_TEST);
			glBlendFunc(GL_ONE, GL_ONE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// From left aux buffer with left color transform
			glReadBuffer(GL_AUX0);
			glMatrixMode(GL_COLOR);
			glLoadMatrixf(colorMatrixLeft);
			glMatrixMode(GL_MODELVIEW);
			glCopyPixels(0,0,s.width(),s.height(),GL_COLOR);
			// From right aux buffer with right color transform
			glReadBuffer(GL_AUX1);
			glMatrixMode(GL_COLOR);
			glLoadMatrixf(colorMatrixRight);
			glMatrixMode(GL_MODELVIEW);
			glCopyPixels(0,0,s.width(),s.height(),GL_COLOR);
			// Restore color matrix
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
			glMatrixMode(GL_COLOR);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
		}
		else if (style_ == sidebyside)
		{
			setSide(right);
			glViewport(s.width()/2,0, s.width()/2, s.height());
			scene_->camera()->loadMatrix(this);
			scene_->draw(this);
			glViewport(0,0, s.width(), s.height());
		}
		else if (style_ == quad)
		{
			setSide(right);
			glDrawBuffer(GL_BACK_RIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			scene_->camera()->loadMatrix(this);
			scene_->draw(this);
			glDrawBuffer(GL_BACK_LEFT);
		}
		else if (style_ == line)
		{
			setSide(right);
			glClear(GL_DEPTH_BUFFER_BIT);
			glStencilFunc(GL_EQUAL, 0x01, 0x01);
			scene_->camera()->loadMatrix(this);
			scene_->draw(this);
			glDisable(GL_STENCIL_TEST);
		}
	}
}

static const GLubyte stipplePattern[] = {
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff
};

void GLWidget::lineStencil()
{
	// Prepare projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// Prepare stencil
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 0x01, 0x01);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
	// Lock buffers
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glDepthMask(GL_FALSE);
	// Draw The pattern into the stencil buffer
	glColor4f(1,1,1,1);
	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple(stipplePattern);
	glRectf(0,0,1,1);
	glDisable(GL_POLYGON_STIPPLE);
	// Unlock buffers
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDepthMask(GL_TRUE);
	// Disable stencil
	glStencilFunc(GL_EQUAL, 0x01, 0x01);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glDisable(GL_STENCIL_TEST);
	// Restore projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}

}
