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

namespace Examination
{

const QString FragShaderRenderer::defaultShader = ":/interlace.fs";

FragShaderRenderer::FragShaderRenderer(std::tr1::shared_ptr<Scene> scene, QString shaderPath)
	: AbstractRenderer(scene)
{
	shader_.loadFragmentShader(shaderPath);
	uniform_ = shader_.getUniformLocation("side");
}

void FragShaderRenderer::renderScene(GLWidget * w)
{
	shader_.use();

	shader_.setUniform1f(uniform_, 0.0f);
	w->setSide(GLWidget::left);
	scene()->camera()->loadMatrix(w);
	scene()->draw(w);

	shader_.setUniform1f(uniform_, 1.0f);
	w->setSide(GLWidget::right);
	scene()->camera()->loadMatrix(w);
	scene()->draw(w);

	shader_.disable();
}


}
