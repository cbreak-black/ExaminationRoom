/*
 *  abstractrenderer.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "abstractrenderer.h"

namespace Examination
{

AbstractRenderer::AbstractRenderer(std::tr1::shared_ptr<Scene> scene)
{
	scene_ = scene;
}

AbstractRenderer::~AbstractRenderer()
{
}

std::tr1::shared_ptr<Scene> AbstractRenderer::scene() const
{
	return scene_;
}

void AbstractRenderer::setScene(std::tr1::shared_ptr<Scene> scene)
{
	scene_ = scene;
}

}
