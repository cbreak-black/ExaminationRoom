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

#include <QImage.h>

using namespace std::tr1;

namespace Examination
{

Stereogram::Stereogram()
{
	texDepth_.reset();
	texLeft_.reset();
	texRight_.reset();
	zoomFactor_ = 1.0f;
	offset_ = 0;
}

Stereogram::Stereogram(shared_ptr<Texture> l, shared_ptr<Texture> r)
{
	texDepth_.reset();
	texLeft_ = l;
	texRight_ = r;
	zoomFactor_ = 1.0f;
	offset_ = 0;
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

float Stereogram::zoom()
{
	return zoomFactor_;
}

void Stereogram::setZoom(float z)
{
	zoomFactor_ = z;
	if (texDepth_)
		texDepth_->setZoom(z);
	recreateStereogram();
	if (texLeft_)
		texLeft_->setZoom(z);
	if (texRight_)
		texRight_->setZoom(z);
}

int Stereogram::offset()
{
	return offset_;
}

void Stereogram::setOffset(int o)
{
	offset_ = o;
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
		texDepth_->setZoom(zoomFactor_);
	recreateStereogram();
}

void Stereogram::setTexLeft(std::tr1::shared_ptr<Texture> t)
{
	texLeft_ = t;
	if (texLeft_)
		texLeft_->setZoom(zoomFactor_);
}

void Stereogram::setTexRight(std::tr1::shared_ptr<Texture> t)
{
	texRight_ = t;
	if (texRight_)
		texRight_->setZoom(zoomFactor_);
}

}
