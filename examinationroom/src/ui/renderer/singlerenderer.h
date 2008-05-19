/*
 *  singlerenderer.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "abstractrenderer.h"

namespace Examination
{

class SingleRenderer : public AbstractRenderer
{
public:
	SingleRenderer(std::tr1::shared_ptr<Scene> scene);

public:
	virtual void renderScene(GLWidget * w);
};

}
