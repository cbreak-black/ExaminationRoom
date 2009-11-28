/*
 *  fragshaderrenderer.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "fragshaderrenderer.h"

#include "scene.h"
#include "camera.h"
#include "glwidget.h"

#include <QGLFramebufferObject>

namespace Examination
{

const QString FragShaderRenderer::defaultShader = ":/interlace.fs";

FragShaderRenderer::FragShaderRenderer(std::tr1::shared_ptr<Scene> scene, QString shaderPath)
	: AbstractRenderer(scene)
{
	shader_.loadFragmentShader(shaderPath);
	uniform_ = shader_.getUniformLocation("side");

	texSize_ = QSize(0,0);

	texL_ = NULL;
	texR_ = NULL;
}

FragShaderRenderer::~FragShaderRenderer()
{
	if (texL_) delete texL_;
	if (texR_) delete texR_;
}

void FragShaderRenderer::renderScene(GLWidget * w)
{
	QSize s = w->size();
	if (s != texSize_)
	{
		if (texL_) delete texL_;
		if (texR_) delete texR_;

		// Only works when there's a current GL Context, which is the case
		// Note that this only works with at least OpenGL 2.0, since on older versions
		// GL_TEXTURE_2D requires power-of-two sizes
		texL_ = new QGLFramebufferObject(s, QGLFramebufferObject::CombinedDepthStencil);
		texR_ = new QGLFramebufferObject(s, QGLFramebufferObject::CombinedDepthStencil);
		texSize_ = s;
	}

	// Prepare left FB
	if (texL_->bind())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		w->setSide(GLWidget::left);
		scene()->camera()->loadMatrix(w);
		scene()->draw(w);
		texL_->release();
	}

	// Prepare right FB
	if (texR_->bind())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		w->setSide(GLWidget::right);
		scene()->camera()->loadMatrix(w);
		scene()->draw(w);
		texR_->release();
	}

	// Prepare for drawing textures
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// Set up camera
	glOrtho(0,1, 0,1, -1,1);

	// Since the shaders have no lighting or anything, no need to disable it
	shader_.use();

	shader_.setUniform1f(uniform_, 0.0f);
	// Draw Left FB
	drawFB(texL_);

	shader_.setUniform1f(uniform_, 1.0f);
	// Draw Right FB
	drawFB(texR_);

	shader_.disable();

	// And restore the matrix stack
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void FragShaderRenderer::drawFB(QGLFramebufferObject * tex)
{
	glBindTexture(GL_TEXTURE_2D, tex->texture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(0.0f, 1.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}


}
