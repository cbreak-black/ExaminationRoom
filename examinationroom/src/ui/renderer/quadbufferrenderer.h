/*
 *  quadbufferrenderer.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "abstractrenderer.h"

#include "scene.h"
#include "camera.h"
#include "glwidget.h"

namespace Examination
{

class QuadbufferRenderer : public AbstractRenderer
{
public:
	QuadbufferRenderer(std::tr1::shared_ptr<Scene> scene);

public:
	virtual void renderScene(GLWidget * w);
};

}
