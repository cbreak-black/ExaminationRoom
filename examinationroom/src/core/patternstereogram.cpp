/*
 *  patternstereogram.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 19.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "patternstereogram.h"

#include "texture.h"
#include "glwidget.h"

#include <QImage.h>

#include <cstdlib>

#include "platform_math.h" // for roundf

using namespace std::tr1;

namespace Examination
{

PatternStereogram::PatternStereogram(shared_ptr<Texture> d, shared_ptr<Texture> p)
{
	texPattern_ = p;
	setTexDepth(d);
	// Stereogram gets generated automatically
}

void PatternStereogram::recreateStereogram()
{
	QImage imageTemp = texDepth()->image();
	QImage imagePattern = texPattern_->image();
	QSize depthSize = imageTemp.size();
	QSize patSize = imagePattern.size();

	const int offset = 4;
	const int divisor = 255 / offset;
	int i, j;
	
	// Create left and right tex
	// TODO: Select random subset
	int targetSizeX = patSize.width() - depthSize.width() - offset;
	int targetSizeY = patSize.height() - depthSize.height();
	if (targetSizeX < 0 || targetSizeY < 0) return; // Error
	int targetX = rand() % targetSizeX;
	int targetY = rand() % targetSizeY;
	QRect r = QRect(targetX+offset, targetY, depthSize.width(), depthSize.height());
	QImage imageL = imagePattern.copy(r);
	QImage imageR = QImage(depthSize, imagePattern.format());

	unsigned int c;		// Color
	unsigned char o;	// Offset
	if (imagePattern.depth() > 8)
	{
		for (j = 0; j < depthSize.height(); j++)
		{
			for (i = 0; i < depthSize.width(); i++)
			{
				o = roundf(((float)qGray(imageTemp.pixel(i, j))) / divisor);
				c = imagePattern.pixel(i-o+r.x(), j+r.y());
				imageR.setPixel(i, j, c);
			}
		}
	}
	else
	{
		for (j = 0; j < depthSize.height(); j++)
		{
			for (i = 0; i < depthSize.width(); i++)
			{
				o = roundf(((float)qGray(imageTemp.pixel(i, j))) / divisor);
				c = imagePattern.pixelIndex(i-o+r.x(), j+r.y());
				imageR.setPixel(i, j, c);
			}
		}
	}

	shared_ptr<Texture> texLeft = shared_ptr<Texture>(new Texture(imageL));
	shared_ptr<Texture> texRight = shared_ptr<Texture>(new Texture(imageR));
	setTexLeft(texLeft);
	setTexRight(texRight);
}

}
