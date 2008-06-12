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

#include <QImage>

#include "platform_math.h" // for roundf

#include "parameter/parameterrandomdot.h"

using namespace std::tr1;

namespace Examination
{

RandomdotStereogram::RandomdotStereogram(shared_ptr<Texture> d)
{
	setMaxColor(4);
	setExclusiveColor(0);
	setOffset(6);
	setTexDepth(d);
	// Stereogram gets generated automatically
}

RandomdotStereogram::RandomdotStereogram(const std::string & path)
{
	setMaxColor(4);
	setExclusiveColor(0);
	setOffset(6);
	setTexDepth(shared_ptr<Texture>(new Texture(path)));
	// Stereogram gets generated automatically
}

std::tr1::shared_ptr<AbstractTexture> RandomdotStereogram::clone() const
{
	return std::tr1::shared_ptr<AbstractTexture>(new RandomdotStereogram(*this));
}

void RandomdotStereogram::recreateStereogram()
{
	if (!texDepth()) return;
	if (!texDepth()->valid()) return;
	QImage imageTemp = texDepth()->image();
	QSize s = imageTemp.size();

	// Create left and right tex
	QImage imageL = QImage(s, QImage::Format_Indexed8);
	QImage imageR = QImage(s, QImage::Format_Indexed8);

	int i, j;
	// Color Map
	imageL.setNumColors(maxColor_);
	imageR.setNumColors(maxColor_);
	for (i = 0; i < maxColor_; i++)
	{
		QRgb c = colors_[i];
		imageL.setColor(i, c);
		imageR.setColor(i, c);
	}

	const int divisor = 255 / offset();

	// Random BG
	for (j = 0; j < s.height(); j++)
	{
		for (i = 0; i < s.width(); i++)
		{
			char col = rand()%abs(maxColor_-excColor_);
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
			QRgb c = colors_[i];
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

int RandomdotStereogram::maxColor() const
{
	return maxColor_;
}

int RandomdotStereogram::exclusiveColor() const
{
	return excColor_;
}

void RandomdotStereogram::setMaxColor(int mc)
{
	maxColor_ = abs(mc < 127 ? mc : 127);
	excColor_ = excColor_ < maxColor_ ? excColor_ : 0;
	resetColor();
	recreateStereogram();
}

void RandomdotStereogram::setExclusiveColor(int ec)
{
	excColor_ = abs(ec < maxColor_ ? ec : 0);
	recreateStereogram();
}

void RandomdotStereogram::setColor(int idx, char r, char g, char b)
{
	setColor(idx, qRgb(r, g, b));
}

void RandomdotStereogram::setColor(int idx, unsigned int color)
{
	if (idx < maxColor_ && idx >= 0)
	{
		colors_[idx] = color;
		applyColorPalette();
	}
}

void RandomdotStereogram::resetColor()
{
	colors_.clear();
	float step = 255.0/(maxColor_-1);
	for (int i = 0; i < maxColor_; i++)
	{
		colors_.push_back(qRgb(i*step, i*step, i*step));
	}
	applyColorPalette();
}

void RandomdotStereogram::applyColorPalette()
{
	if (texLeft() && texLeft()->valid())
	{
		QImage image = texLeft()->image();
		image.setNumColors(maxColor_);
		for (int i = 0; i < maxColor_; i++)
		{
			image.setColor(i, colors_[i]);
		}
	}
	if (texRight() && texRight()->valid())
	{
		QImage image = texRight()->image();
		image.setNumColors(maxColor_);
		for (int i = 0; i < maxColor_; i++)
		{
			image.setColor(i, colors_[i]);
		}
	}
}

std::tr1::shared_ptr<Parameterdialog> RandomdotStereogram::createDialog()
{
	return std::tr1::shared_ptr<Parameterdialog>(new ParameterRandomDot(sharedPtr()));
}

// Serialisation
std::string RandomdotStereogram::className() const
{
	return "RandomDot";
}

std::string RandomdotStereogram::toLua(std::ostream & outStream) const
{
	// Let parent create an instance and set parent parameters
	std::string name = Stereogram::toLua(outStream);
	// Set own parameters
	outStream << name << ":" << "setMaxColor(" << maxColor_ << ");\n";
	outStream << name << ":" << "setExclusiveColor(" << excColor_ << ");\n";
	for (unsigned int i = 0; i < colors_.size(); i++)
	{
		outStream << name << ":" << "setColor(" << i << ", "
			<< qRed(colors_[i]) << ", " << qGreen(colors_[i]) << ", " << qBlue(colors_[i]) << ");\n";
	}
	return name;
}

std::string RandomdotStereogram::toLuaCreate(std::ostream & outStream) const
{
	std::string name = "tex";
	outStream << name << " = " << className() << "(\"" << texDepth()->path() << "\");\n";
	return name;
}

}
