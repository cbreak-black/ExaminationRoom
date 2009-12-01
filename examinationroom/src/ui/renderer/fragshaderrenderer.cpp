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

#include "errortool.h"

#include <QGLFramebufferObject>

namespace Examination
{

const QString FragShaderRenderer::defaultShader = ":/interlace.fs";

FragShaderRenderer::FragShaderRenderer(std::tr1::shared_ptr<Scene> scene, QString shaderPath)
	: AbstractRenderer(scene)
{
	shader_.loadFragmentShader(shaderPath);
	uniformSide_ = shader_.getUniformLocation("side");

	// For Mayan shader, hardcoded for now (ignored if shader has no lambda uniform)
	shader_.setUniform1f(shader_.getUniformLocation("lambda"), 0.6f);

	texSize_ = QSize(0,0);

	texL_ = NULL;
	texR_ = NULL;
	ErrorTool::getErrors("FragShaderRenderer::FragShaderRenderer");
}

FragShaderRenderer::~FragShaderRenderer()
{
	if (texL_) delete texL_;
	if (texR_) delete texR_;
}

void FragShaderRenderer::createFBO(QSize s)
{
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
	ErrorTool::getErrors("FragShaderRenderer::createFBO");
}

void FragShaderRenderer::renderFBO(GLWidget * w, QGLFramebufferObject * tex)
{
	if (tex->bind())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene()->camera()->loadMatrix(w);
		scene()->draw(w);
		tex->release();
	}
	ErrorTool::getErrors("FragShaderRenderer::renderFBO");
}

void FragShaderRenderer::renderScene(GLWidget * w)
{
	createFBO(w->size());
	// Prepare left FB
	w->setSide(GLWidget::left);
	renderFBO(w, texL_);
	// Prepare right FBO
	w->setSide(GLWidget::right);
	renderFBO(w, texR_);

	// Shaders just add the two textures onto each other
	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_DEPTH_TEST);

	// Prepare for drawing textures
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// Set up camera
	glOrtho(0,1, 0,1, -1,1);

	// Clear to black not to custom BG
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Since the shaders have no lighting or anything, no need to disable it
	shader_.use();

	shader_.setUniform1f(uniformSide_, 0.0f);
	// Draw Left FB
	drawFB(texL_);

	shader_.setUniform1f(uniformSide_, 1.0f);
	// Draw Right FB
	drawFB(texR_);

	shader_.disable();

	// And restore the matrix stack
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// Restore blend func
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ErrorTool::getErrors("FragShaderRenderer::renderScene");
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
	ErrorTool::getErrors("FragShaderRenderer::drawFB");
}


}
