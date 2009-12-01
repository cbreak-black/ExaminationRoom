/*
 *  singlerenderer.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "singlerenderer.h"

#include "scene.h"
#include "camera.h"
#include "glwidget.h"

#include "errortool.h"

namespace Examination
{

SingleRenderer::SingleRenderer(std::tr1::shared_ptr<Scene> scene)
	: AbstractRenderer(scene)
{
}

void SingleRenderer::renderScene(GLWidget * w)
{
	scene()->camera()->loadMatrix(w);
	scene()->draw(w);
	ErrorTool::getErrors("SingleRenderer::renderScene");
}

}
