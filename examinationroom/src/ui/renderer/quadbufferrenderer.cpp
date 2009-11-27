/*
 *  quadbufferrenderer.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "quadbufferrenderer.h"

#include "scene.h"
#include "camera.h"
#include "glwidget.h"

namespace Examination
{

QuadbufferRenderer::QuadbufferRenderer(std::tr1::shared_ptr<Scene> scene)
	: AbstractRenderer(scene)
{
}

void QuadbufferRenderer::renderScene(GLWidget * w)
{
	// This requires stereo buffering to be enabled in the OpenGL context

	w->setSide(GLWidget::left);
	scene()->camera()->loadMatrix(w);
	scene()->draw(w);

	glDrawBuffer(GL_BACK_RIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	w->setSide(GLWidget::right);
	scene()->camera()->loadMatrix(w);
	scene()->draw(w);

	glDrawBuffer(GL_BACK_LEFT);
}

}