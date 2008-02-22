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

/**
The number of colors used in random dot stereograms.
*/
const int maxColor = 4; // Number of grayscales in image
const int excColor = 0; // Number of colors exclusively to fg/bg

RandomdotStereogram::RandomdotStereogram(shared_ptr<Texture> d)
{
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
	float step = 255.0/(maxColor-1);
	imageL.setNumColors(maxColor);
	imageR.setNumColors(maxColor);
	for (i = 0; i < maxColor; i++)
	{
		QRgb c = qRgb(i*step, i*step, i*step);
		imageL.setColor(i, c);
		imageR.setColor(i, c);
	}

	const int offset = 6;
	const int divisor = 255 / offset;

	for (j = 0; j < s.height(); j++)
	{
		for (i = 0; i < s.width(); i++)
		{
			char col = rand()%(maxColor-excColor);
			imageL.setPixel(i, j, col);
			imageR.setPixel(i, j, col);
		}
	}

	for (j = 0; j < s.height(); j++)
	{
		for (i = 0; i < s.width(); i++)
		{
			char col = rand()%(maxColor-excColor)+excColor;
			unsigned char o = roundf(((float)qGray(imageTemp.pixel(i, j))) / divisor);
			if (o > 0)
			{
				unsigned char oL = floor(0.5f*o);
				unsigned char oR = o - oL;
				if (i+oL < s.width())
					imageL.setPixel(i+oL, j, col);
				if (i-oR > 0)
					imageR.setPixel(i-oR, j, col);
			}
		}
	}

	shared_ptr<Texture> texLeft = shared_ptr<Texture>(new Texture(imageL));
	shared_ptr<Texture> texRight = shared_ptr<Texture>(new Texture(imageR));
	setTexLeft(texLeft);
	setTexRight(texRight);
}

}
