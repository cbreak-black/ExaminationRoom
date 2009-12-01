/*
 *  anaglyphrenderer.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "anaglyphrenderer.h"

#include "scene.h"
#include "camera.h"
#include "glwidget.h"

#include "errortool.h"

namespace Examination
{

AnaglyphRenderer::AnaglyphRenderer(std::tr1::shared_ptr<Scene> scene)
	: AbstractRenderer(scene)
{
}

void AnaglyphRenderer::renderScene(GLWidget * w)
{
	w->setSide(GLWidget::left);
	//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glColorMask(1, 0, 0, 1);
	scene()->camera()->loadMatrix(w);
	scene()->draw(w);

	glClear(GL_DEPTH_BUFFER_BIT);

	w->setSide(GLWidget::right);
	//glColor4f(0.0f, 1.0f, 1.0f, 0.5f);
	glColorMask(0, 1, 1, 1);
	scene()->camera()->loadMatrix(w);
	scene()->draw(w);

	glColorMask(1, 1, 1, 1);

	ErrorTool::getErrors("AnaglyphRenderer::renderScene");
}

}
