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

#include <QImage>

#include <cstdlib>

#include "platform_math.h" // for roundf

using namespace std::tr1;

namespace Examination
{

PatternStereogram::PatternStereogram(shared_ptr<Texture> d, shared_ptr<Texture> p)
{
	bgPattern_ = p;
	fgPattern_ = p;
	setTexDepth(d);
	// Stereogram gets generated automatically
}

PatternStereogram::PatternStereogram(shared_ptr<Texture> d, shared_ptr<Texture> p, shared_ptr<Texture> q)
{
	bgPattern_ = p;
	fgPattern_ = q;
	setTexDepth(d);
	// Stereogram gets generated automatically
}

void PatternStereogram::recreateStereogram()
{
	if (!texDepth()) return;
	QImage imageTemp = texDepth()->image();
	QImage imageBG = bgPattern_->image();
	QImage imageFG = fgPattern_->image();
	QSize s = imageTemp.size();
	QSize bgSize = imageBG.size();
	QSize fgSize = imageFG.size();

	// Create left and right tex rectangle for bg
	int targetX = rand() % bgSize.width();
	int targetY = rand() % bgSize.height();
	QRect bgR = QRect(targetX, targetY, s.width(), s.height());
	// Create left and right tex rectangle for fg
	targetX = rand() % fgSize.width();
	targetY = rand() % fgSize.height();
	QRect fgR = QRect(targetX, targetY, s.width(), s.height());
	// Set Up BG
	QImage imageL = QImage(s, QImage::Format_RGB32);
	QImage imageR = QImage(s, QImage::Format_RGB32);
	for (int j = 0; j < s.height(); j++)
	{
		for (int i = 0; i < s.width(); i++)
		{
			QRgb col = imageBG.pixel(i%imageBG.width(), j%imageBG.height());
			imageL.setPixel(i, j, col);
			imageR.setPixel(i, j, col);
		}
	}

	const int divisor = 255 / offset();
	if (style() == Stereogram::convex)
	{
		for (int j = 0; j < s.height(); j++)
		{
			for (int i = 0; i < s.width(); i++)
			{
				unsigned char o = roundf(((float)qGray(imageTemp.pixel(i, j))) / divisor);
				if (o > 0)
				{
					// For convex objects, the destination in the picture is offset
					QRgb col = imageFG.pixel(i%imageFG.width(), j%imageFG.height());
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
		for (int j = 0; j < s.height(); j++)
		{
			for (int i = 0; i < s.width(); i++)
			{
				unsigned char o = roundf(((float)qGray(imageTemp.pixel(i, j))) / divisor);
				if (o > 0)
				{
					// For concave objects, the source of the picture is offset
					imageL.setPixel(i, j, imageFG.pixel((i+o)%imageFG.width(), j%imageFG.height()));
					imageR.setPixel(i, j, imageFG.pixel(i%imageFG.width(), j%imageFG.height()));
				}
			}
		}
	}

	shared_ptr<Texture> texLeft = shared_ptr<Texture>(new Texture(imageL));
	shared_ptr<Texture> texRight = shared_ptr<Texture>(new Texture(imageR));
	setTexLeft(texLeft);
	setTexRight(texRight);
}

// Serialisation
std::string PatternStereogram::className() const
{
	return "Pattern";
}

std::string PatternStereogram::toLua(std::ostream & outStream) const
{
	// Let parent create an instance and set parent parameters
	std::string name = Stereogram::toLua(outStream);
	// Set own parameters
	return name;
}

/**
 \todo	Remove evil global variable name usage once the program/namemanager are implemented
*/
std::string PatternStereogram::toLuaCreate(std::ostream & outStream) const
{
	std::string name = "tex";
	outStream << name << " = Texture(\"" << className() << "\", \"" << texDepth()->path() << "\",\n\"";
	if (fgPattern_->path() == bgPattern_->path())
	{
		outStream << bgPattern_->path() << ");\n";
	}
	else
	{
		outStream << bgPattern_->path() << "\", \"" << fgPattern_->path() << "\");\n";
	}
	return name;
}

}
