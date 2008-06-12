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

#include "parameter/parameterpatternstereogram.h"

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

PatternStereogram::PatternStereogram(const std::string & d, const std::string & b, const std::string & f)
{
	bgPattern_ = shared_ptr<Texture>(new Texture(d));
	fgPattern_ = shared_ptr<Texture>(new Texture(b));
	setTexDepth(shared_ptr<Texture>(new Texture(f)));
	// Stereogram gets generated automatically
}

PatternStereogram::PatternStereogram(const PatternStereogram & p)
	: Stereogram(p)
{
	bgPattern_ = std::tr1::dynamic_pointer_cast<Texture>(p.bgPattern_->clone());
	fgPattern_ = std::tr1::dynamic_pointer_cast<Texture>(p.fgPattern_->clone());
}

std::tr1::shared_ptr<AbstractTexture> PatternStereogram::clone() const
{
	return std::tr1::shared_ptr<PatternStereogram>(new PatternStereogram(*this));
}

void PatternStereogram::recreateStereogram()
{
	if ((!texDepth()) || (!texDepth()->valid())) return;
	if ((!texBG()) || (!texBG()->valid())) return;
	if ((!texFG()) || (!texFG()->valid())) return;
	QImage imageTemp = texDepth()->image();
	QImage imageBG = texBG()->image();
	QImage imageFG = texFG()->image();
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

std::tr1::shared_ptr<Texture> PatternStereogram::texBG() const
{
	return bgPattern_;
}

std::tr1::shared_ptr<Texture> PatternStereogram::texFG() const
{
	return fgPattern_;
}

std::tr1::shared_ptr<Parameterdialog> PatternStereogram::createDialog()
{
	return std::tr1::shared_ptr<Parameterdialog>(new ParameterPatternStereogram(sharedPtr()));
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

std::string PatternStereogram::toLuaCreate(std::ostream & outStream) const
{
	std::string name = "tex";
	outStream << name << " = " << className() << "(\"" << texDepth()->path() << "\",\n\t\"";
	outStream << bgPattern_->path() << "\", \"" << fgPattern_->path() << "\");\n";
	return name;
}

}
