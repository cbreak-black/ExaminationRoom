/*
 *  stereogram.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "stereogram.h"

#include "texture.h"
#include "glwidget.h"

namespace Examination
{

Stereogram::Stereogram(Texture* d)
{
	texDepth_ = d;
}

Stereogram::Stereogram(Texture* l, Texture* r)
{
	texDepth_ = 0;
	
	texLeft_ = l;
	texRight_ = r;
}

Stereogram::~Stereogram()
{
	delete texDepth_; texDepth_ = 0;
	delete texLeft_; texLeft_ = 0;
	delete texRight_; texRight_ = 0;
}

void Stereogram::glBindTex(GLWidget * w)
{
	if (texRight_ && w->side() == right)
	{
		texRight_->glBindTex(w);
	}
	else if (texLeft_)
	{
		texLeft_->glBindTex(w);
	}
}


}
