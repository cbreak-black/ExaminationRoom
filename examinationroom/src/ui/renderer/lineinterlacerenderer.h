/*
 *  lineinterlacerenderer.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "abstractrenderer.h"

namespace Examination
{

class LineInterlaceRenderer : public AbstractRenderer
{
public:
	LineInterlaceRenderer(std::tr1::shared_ptr<Scene> scene);

public:
	virtual void renderScene(GLWidget * w);

private:
	/**
	Fills the stencil buffer with alternating lines.
	*/
	void lineStencil();
};

}
