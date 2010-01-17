/*
 *  sidebysiderenderer.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "sidebysiderenderer.h"

#include "scene.h"
#include "camera.h"
#include "glwidget.h"

#include "glerrortool.h"

namespace Examination
{

SideBySideRenderer::SideBySideRenderer(std::tr1::shared_ptr<Scene> scene)
	: AbstractRenderer(scene)
{
}

void SideBySideRenderer::renderScene(GLWidget * w)
{
	QSize s = w->size();

	w->setSide(GLWidget::left);
	glViewport(0,0, s.width()/2, s.height());
	scene()->draw(w);

	w->setSide(GLWidget::right);
	glViewport(s.width()/2,0, s.width()/2, s.height());
	scene()->draw(w);

	glViewport(0,0, s.width(), s.height());

	GlErrorTool::getErrors("SideBySideRenderer::renderScene");
}

}
