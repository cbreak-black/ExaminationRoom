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
const int maxColor = 2;

RandomdotStereogram::RandomdotStereogram(shared_ptr<Texture> d)
{
	setTexDepth(d);
	// Stereogram gets generated automatically
}

void RandomdotStereogram::recreateStereogram()
{
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

	const int offset = 4;
	const int divisor = 255 / offset;
	unsigned char history[offset+1];
	unsigned char historyBase = 0;
	for (i = 0; i < offset+1; i++)
		history[i] = rand()%maxColor;

	for (j = 0; j < s.height(); j++)
	{
		for (i = 0; i < s.width(); i++)
		{
			char col = rand()%maxColor;
			history[historyBase] = col;
			unsigned char o = roundf(((float)qGray(imageTemp.pixel(i, j))) / divisor);
			imageL.setPixel(i, j, col);
			imageR.setPixel(i, j, history[(historyBase+o)%(offset+1)]);
			if (historyBase == 0) historyBase = offset;
			else historyBase--;
		}
	}

	shared_ptr<Texture> texLeft = shared_ptr<Texture>(new Texture(imageL));
	shared_ptr<Texture> texRight = shared_ptr<Texture>(new Texture(imageR));
	setTexLeft(texLeft);
	setTexRight(texRight);
}

}
