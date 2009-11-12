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
	_shader.loadFragmentShader(shaderPath);
	_uniformVar = _shader.getUniformLocation("side");
}

void FragShaderRenderer::renderScene(GLWidget * w)
{
	_shader.use();

	_shader.setUniform1f(_uniformVar, 0.0f);
	w->setSide(GLWidget::left);
	scene()->camera()->loadMatrix(w);
	scene()->draw(w);

	_shader.setUniform1f(_uniformVar, 1.0f);
	w->setSide(GLWidget::right);
	scene()->camera()->loadMatrix(w);
	scene()->draw(w);

	_shader.disable();
}


}
