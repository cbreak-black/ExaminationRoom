/*
 *  abstracttexture.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "abstracttexture.h"

namespace Examination
{

AbstractTexture::AbstractTexture()
{
	zoomFactorX_ = 1.0f;
	zoomFactorY_ = 1.0f;
}

AbstractTexture::~AbstractTexture()
{
}

Tool::Vec2f AbstractTexture::zoom() const
{
	return Tool::Vec2f(zoomFactorX_, zoomFactorY_);
}

void AbstractTexture::setZoom(float zx, float zy)
{
	zoomFactorX_ = zx;
	zoomFactorY_ = zy;
}

void AbstractTexture::setZoom(const Tool::Vec2f & z)
{
	zoomFactorX_ = z.x;
	zoomFactorY_ = z.y;
}

}