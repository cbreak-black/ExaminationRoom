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

#include "renderer/abstractrenderer.h"
#include "renderer/anaglyphrenderer.h"
#include "renderer/lineinterlacerenderer.h"
#include "renderer/matrixrenderer.h"
#include "renderer/quadbufferrenderer.h"
#include "renderer/sidebysiderenderer.h"
#include "renderer/singlerenderer.h"

#include "errortool.h"

namespace Examination
{

const Tool::Color4 black(0,0,0,1);

GLWidget::GLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
{
	ErrorTool::getErrors("GLWidget::GLWidget:1");
	setSide(left);
	setStyle(single);
	this->setCursor(Qt::CrossCursor);
	ErrorTool::getErrors("GLWidget::GLWidget:2");
}

GLWidget::GLWidget(const QGLFormat & format, QWidget *parent, QGLWidget *shareWidget)
	: QGLWidget(format, parent, shareWidget)
{
	ErrorTool::getErrors("GLWidget::GLWidget:1");
	setSide(left);
	setStyle(single);
	this->setCursor(Qt::BlankCursor);
	ErrorTool::getErrors("GLWidget::GLWidget:2");
}

GLWidget::~GLWidget()
{
	ErrorTool::getErrors("GLWidget::~GLWidget");
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
	ErrorTool::getErrors("GLWidget::setScene:1");
	scene_ = s;
	renderer_->setScene(s);
	ErrorTool::getErrors("GLWidget::setScene:2");
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
	ErrorTool::getErrors("GLWidget::setStyle:1");
	switch (s)
	{
		case single:
			renderer_ = std::tr1::shared_ptr<AbstractRenderer>(new SingleRenderer(scene_));
			break;
		case anaglyph:
			renderer_ = std::tr1::shared_ptr<AbstractRenderer>(new AnaglyphRenderer(scene_));
			break;
		case matrix:
			GLint auxNum;
			glGetIntegerv(GL_AUX_BUFFERS, &auxNum);
			if (auxNum < 2)
			{
				ErrorTool::logError("GLWidget::setStyle",
									"Not enough aux buffers support found. Matrix stereo not enabled");
				return;
			}
			renderer_ = std::tr1::shared_ptr<AbstractRenderer>(new MatrixRenderer(scene_));
			break;
		case sidebyside:
			renderer_ = std::tr1::shared_ptr<AbstractRenderer>(new SideBySideRenderer(scene_));
			break;
		case quad:
			if (!format().stereo())
			{
				ErrorTool::logError("GLWidget::setStyle",
									"No Stereo support found. Quad Buffer not enabled");
				return;
			}
			renderer_ = std::tr1::shared_ptr<AbstractRenderer>(new QuadbufferRenderer(scene_));
			break;
		case line:
			renderer_ = std::tr1::shared_ptr<AbstractRenderer>(new LineInterlaceRenderer(scene_));
			break;
	}
	style_ = s;
	ErrorTool::getErrors("GLWidget::setStyle:2");
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

	ErrorTool::getErrors("GLWidget::initializeGL");
}

void GLWidget::paintGL()
{
	// Scene
	if (scene_)
	{
		Tool::Vec4f c = scene_->backgroundColor();
		glClearColor(c.r, c.g, c.b, c.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer_->renderScene(this);
	}
	ErrorTool::getErrors("GLWidget::paintGL");
}

void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	ErrorTool::getErrors("GLWidget::resizeGL");
}

}
