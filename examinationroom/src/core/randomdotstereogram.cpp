/*
 *  randomdotstereogram.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 19.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "randomdotstereogram.h"

#include "texture.h"
#include "glwidget.h"

#include <QImage.h>

#include <cstdlib>

#include "platform_math.h" // for roundf

using namespace std::tr1;

namespace Examination
{

RandomdotStereogram::RandomdotStereogram(shared_ptr<Texture> d)
{
	maxColor_ = 4;
	excColor_ = 0;
	setOffset(6);
	setTexDepth(d);
	// Stereogram gets generated automatically
}

void RandomdotStereogram::recreateStereogram()
{
	if (!texDepth()) return;
	QImage imageTemp = texDepth()->image();
	QSize s = imageTemp.size();

	// Create left and right tex
	QImage imageL = QImage(s, QImage::Format_Indexed8);
	QImage imageR = QImage(s, QImage::Format_Indexed8);

	int i, j;
	// Color Map
	float step = 255.0/(maxColor_-1);
	imageL.setNumColors(maxColor_);
	imageR.setNumColors(maxColor_);
	for (i = 0; i < maxColor_; i++)
	{
		QRgb c = qRgb(i*step, i*step, i*step);
		imageL.setColor(i, c);
		imageR.setColor(i, c);
	}

	const int divisor = 255 / offset();

	// Random BG
	for (j = 0; j < s.height(); j++)
	{
		for (i = 0; i < s.width(); i++)
		{
			char col = rand()%(maxColor_-excColor_);
			imageL.setPixel(i, j, col);
			imageR.setPixel(i, j, col);
		}
	}

	// Offset FG
	if (style() == Stereogram::convex)
	{
		for (j = 0; j < s.height(); j++)
		{
			for (i = 0; i < s.width(); i++)
			{
				char col = rand()%(maxColor_-excColor_)+excColor_;
				unsigned char o = roundf(((float)qGray(imageTemp.pixel(i, j))) / divisor);
				if (o > 0)
				{
					// For convex objects, the destination in the picture is offset
					unsigned char oL = floor(0.5f*o);
					unsigned char oR = o - oL;
					if (i+oL < s.width())
						imageL.setPixel(i+oL, j, col);
					if (i-oR > 0)
						imageR.setPixel(i-oR, j, col);
				}
			}
		}
	}
	else // (style() == Stereogram::concave)
	{
		QImage imageS = QImage(s, QImage::Format_Indexed8);
		imageS.setNumColors(maxColor_);
		for (i = 0; i < maxColor_; i++)
		{
			QRgb c = qRgb(i*step, i*step, i*step);
			imageS.setColor(i, c);
		}
		for (j = 0; j < s.height(); j++)
		{
			for (i = 0; i < s.width(); i++)
			{
				char col = rand()%(maxColor_-excColor_)+excColor_;
				imageS.setPixel(i, j, col);
			}
		}
		for (j = 0; j < s.height(); j++)
		{
			for (i = 0; i < s.width(); i++)
			{
				unsigned char o = roundf(((float)qGray(imageTemp.pixel(i, j))) / divisor);
				if (o > 0)
				{
					// For concave objects, the source of the picture is offset
					imageL.setPixel(i, j, imageS.pixelIndex((i+o)%imageS.width(), j));
					imageR.setPixel(i, j, imageS.pixelIndex(i, j));
				}
			}
		}
	}

	shared_ptr<Texture> texLeft = shared_ptr<Texture>(new Texture(imageL));
	shared_ptr<Texture> texRight = shared_ptr<Texture>(new Texture(imageR));
	setTexLeft(texLeft);
	setTexRight(texRight);
}

void RandomdotStereogram::setMaxColor(int mc)
{
	maxColor_ = mc;
}

void RandomdotStereogram::setExclusiveColor(int ec)
{
	excColor_ = ec;
}

}
