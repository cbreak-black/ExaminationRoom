/*
 *  stereogram.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "stereogram.h"

#include "texture.h"
#include "glwidget.h"

#include <QImage>

using namespace std::tr1;

namespace Examination
{

Stereogram::Stereogram()
{
	texDepth_.reset();
	texLeft_.reset();
	texRight_.reset();
	zoomFactorX_ = 1.0f;
	zoomFactorY_ = 1.0f;
	offset_ = 6;
	style_ = convex;
}

Stereogram::Stereogram(shared_ptr<Texture> l, shared_ptr<Texture> r)
{
	texDepth_.reset();
	texLeft_ = l;
	texRight_ = r;
	zoomFactorX_ = 1.0f;
	zoomFactorY_ = 1.0f;
	offset_ = 6;
	style_ = convex;
}

void Stereogram::recreateStereogram()
{
	// Nothing to do here
}

void Stereogram::resizeTo(int width, int height)
{
	if (texDepth_)
	{
		// Automatically generated
		texDepth_->resizeTo(width, height);
		recreateStereogram();
	}
	else
	{
		// Plain stereogram
		texLeft_->resizeTo(width, height);
		texRight_->resizeTo(width, height);
	}
}

void Stereogram::resizeToOriginal()
{
	if (texDepth_)
	{
		// Automatically generated
		texDepth_->resizeToOriginal();
		recreateStereogram();
	}
	else
	{
		// Plain stereogram
		texLeft_->resizeToOriginal();
		texRight_->resizeToOriginal();
	}
}

int Stereogram::width()
{
	if (texLeft_)
		return texLeft_->width();
	else
		return 0;
}

int Stereogram::height()
{
	if (texLeft_)
		return texLeft_->height();
	else
		return 0;
}

float Stereogram::zoomX()
{
	return zoomFactorX_;
}

float Stereogram::zoomY()
{
	return zoomFactorY_;
}

void Stereogram::setZoom(float zx, float zy)
{
	zoomFactorX_ = zx;
	zoomFactorY_ = zy;
	if (texDepth_)
		texDepth_->setZoom(zx, zy);
	recreateStereogram();
	if (texLeft_)
		texLeft_->setZoom(zx, zy);
	if (texRight_)
		texRight_->setZoom(zx, zy);
}

int Stereogram::offset()
{
	return offset_;
}

void Stereogram::setOffset(int o)
{
	if (o > 0)
	{
		offset_ = o;
		recreateStereogram();
	}
}

Stereogram::Style Stereogram::style()
{
	return style_;
}

void Stereogram::setStyle(Stereogram::Style s)
{
	style_ = s;
	recreateStereogram();
}

void Stereogram::glBindTex(GLWidget * w)
{
	if (texRight_ && w->side() == GLWidget::right)
	{
		texRight_->glBindTex(w);
	}
	else if (texLeft_)
	{
		texLeft_->glBindTex(w);
	}
}

void Stereogram::draw(GLWidget * w)
{
	if (texRight_ && w->side() == GLWidget::right)
	{
		texRight_->draw(w);
	}
	else if (texLeft_)
	{
		texLeft_->draw(w);
	}
}

std::tr1::shared_ptr<Texture> Stereogram::texDepth()
{
	return texDepth_;
}

std::tr1::shared_ptr<Texture> Stereogram::texLeft()
{
	return texLeft_;
}

std::tr1::shared_ptr<Texture> Stereogram::texRight()
{
	return texRight_;
}

void Stereogram::setTexDepth(std::tr1::shared_ptr<Texture> t)
{
	texDepth_ = t;
	if (texDepth_)
		texDepth_->setZoom(zoomFactorX_, zoomFactorY_);
	recreateStereogram();
}

void Stereogram::setTexLeft(std::tr1::shared_ptr<Texture> t)
{
	texLeft_ = t;
	if (texLeft_)
		texLeft_->setZoom(zoomFactorX_, zoomFactorY_);
}

void Stereogram::setTexRight(std::tr1::shared_ptr<Texture> t)
{
	texRight_ = t;
	if (texRight_)
		texRight_->setZoom(zoomFactorX_, zoomFactorY_);
}

}
