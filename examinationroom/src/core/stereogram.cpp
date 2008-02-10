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

#include <cstdlib>
#include <ctime>

#include "platform_math.h" // for roundf

using namespace std::tr1;

namespace Examination
{

/**
The number of colors used in random dot stereograms.
*/
const int maxColor = 2;

Stereogram::Stereogram(shared_ptr<Texture> d)
{
	texDepth_ = d;
	recreateRDS();
}

Stereogram::Stereogram(shared_ptr<Texture> l, shared_ptr<Texture> r)
{
	texDepth_.reset();
	texLeft_ = l;
	texRight_ = r;
}

Stereogram::~Stereogram()
{
}

void Stereogram::recreateRDS()
{
	QImage imageTemp = texDepth_->image();
	QSize s = imageTemp.size();

	// Create left and right tex
	QImage imageL = QImage(s, QImage::Format_Mono);
	QImage imageR = QImage(s, QImage::Format_Mono);

	int i, j;
	float step = 255.0/(maxColor-1);
	imageL.setNumColors(maxColor);
	imageR.setNumColors(maxColor);
	for (i = 0; i < maxColor; i++)
	{
		QRgb c = qRgb(i*step, i*step, i*step);
		imageL.setColor(i, c);
		imageR.setColor(i, c);
	}

	for (j = 0; j < s.height(); j++)
	{
		for (i = 0; i < s.width(); i++)
		{
			imageL.setPixel(i, j, rand()%maxColor);
		}
	}

	int offset = 4;
	int divisor = 255 / offset;

	for (j = 0; j < s.height(); j++)
	{
		for (i = 0; i < s.width(); i++)
		{
			int c = qGray(imageTemp.pixel(i, j));
			int k = i - roundf(((float)c) / divisor); 
			if (k < 0 || k >= s.width())
				imageR.setPixel(i, j, rand()%maxColor);
			else
			{
				imageR.setPixel(i, j, imageL.pixelIndex(k, j));
			}
		}
	}

	texLeft_ = shared_ptr<Texture>(new Texture(imageL));
	texRight_ = shared_ptr<Texture>(new Texture(imageR));
}

void Stereogram::resizeTo(int width, int height)
{
	if (texDepth_)
	{
		// RDS
		texDepth_->resizeTo(width, height);
		recreateRDS();
	}
	else
	{
		texLeft_->resizeTo(width, height);
		texRight_->resizeTo(width, height);
	}
}

void Stereogram::resizeToOriginal()
{
	if (texDepth_)
	{
		// RDS
		texDepth_->resizeToOriginal();
	}
	else
	{
		texLeft_->resizeToOriginal();
		texRight_->resizeToOriginal();
	}
}

int Stereogram::width()
{
	return texLeft_->width();
}

int Stereogram::height()
{
	return texLeft_->height();
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


}
