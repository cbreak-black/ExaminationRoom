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

#include <math.h>

using namespace std::tr1;

namespace Examination
{

/**
The number of colors used in random dot stereograms.
*/
const int maxColor = 3;

Stereogram::Stereogram(shared_ptr<Texture> d)
{
	texDepth_ = d;
	
	QImage * imageTemp = d->image();
	QSize s = imageTemp->size();
	
	// Create left and right tex
	QImage * imageL = new QImage(s, QImage::Format_Indexed8);
	QImage * imageR = new QImage(s, QImage::Format_Indexed8);
	
	srand (time(0));

	int i, j;
	float step = 255.0/(maxColor-1);
	
	imageL->setNumColors(maxColor);
	imageR->setNumColors(maxColor);

	for (i = 0; i < maxColor; i++)
	{
		QRgb c = qRgb(i*step, i*step, i*step);
		imageL->setColor(i, c);
		imageR->setColor(i, c);
	}

	for (j = 0; j < s.height(); j++)
	{
		for (i = 0; i < s.width(); i++)
		{
			imageL->setPixel(i, j, rand()%maxColor);
		}
	}

	int offset = 4;
	int divisor = 255 / offset;
	
	for (j = 0; j < s.height(); j++)
	{
		for (i = 0; i < s.width(); i++)
		{
			int c = qGray(imageTemp->pixel(i, j));
			int k = i - roundf(((float)c) / divisor); 
			if (k < 0 || k >= s.width())
				imageR->setPixel(i, j, rand()%maxColor);
			else
			{
				imageR->setPixel(i, j, imageL->pixelIndex(k, j));
			}
		}
	}
	
	texLeft_ = shared_ptr<Texture>(new Texture(imageL));
	texRight_ = shared_ptr<Texture>(new Texture(imageR));
}

Stereogram::Stereogram(shared_ptr<Texture> l, shared_ptr<Texture> r)
{
	texLeft_ = l;
	texRight_ = r;
}

Stereogram::~Stereogram()
{
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
