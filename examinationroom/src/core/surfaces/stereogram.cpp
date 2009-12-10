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

#include <QImage>

#include "parameter/parameterstereogram.h"

using namespace std::tr1;

namespace Examination
{

Stereogram::Stereogram()
{
	texDepth_.reset();
	texLeft_.reset();
	texRight_.reset();
	offset_ = 6;
	style_ = convex;
}

Stereogram::Stereogram(shared_ptr<Texture> l, shared_ptr<Texture> r)
{
	texDepth_.reset();
	texLeft_ = l;
	texRight_ = r;
	offset_ = 6;
	style_ = convex;
}

Stereogram::Stereogram(const std::string & lpath, const std::string & rpath)
{
	texDepth_.reset();
	texLeft_ = shared_ptr<Texture>(new Texture(lpath));
	texRight_ = shared_ptr<Texture>(new Texture(rpath));
	offset_ = 6;
	style_ = convex;
}

Stereogram::Stereogram(const Stereogram & s)
	: AbstractTexture(s)
{
	texDepth_ = std::tr1::dynamic_pointer_cast<Texture>(s.texDepth_->clone());
	texLeft_ = std::tr1::dynamic_pointer_cast<Texture>(s.texLeft_->clone());
	texRight_ = std::tr1::dynamic_pointer_cast<Texture>(s.texRight_->clone());
}

std::tr1::shared_ptr<AbstractTexture> Stereogram::clone() const
{
	return std::tr1::shared_ptr<AbstractTexture>(new Stereogram(*this));
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

int Stereogram::width() const
{
	if (texLeft_)
		return texLeft_->width();
	else
		return 0;
}

int Stereogram::height() const
{
	if (texLeft_)
		return texLeft_->height();
	else
		return 0;
}

void Stereogram::setZoom(float zx, float zy)
{
	AbstractTexture::setZoom(zx, zy);
	if (texDepth_)
		texDepth_->setZoom(zx, zy);
	recreateStereogram();
	if (texLeft_)
		texLeft_->setZoom(zx, zy);
	if (texRight_)
		texRight_->setZoom(zx, zy);
}

void Stereogram::setFilterType(AbstractTexture::FilterType f)
{
	AbstractTexture::setFilterType(f);
	// Only visible textures get their filter type changed
	if (texLeft_)
		texLeft_->setFilterType(f);
	if (texRight_)
		texRight_->setFilterType(f);
}

int Stereogram::offset() const
{
	return offset_;
}

void Stereogram::setOffset(int o)
{
	if (o > 0)
	{
		offset_ = o;
		recreateStereogram();
	}
}

Stereogram::Style Stereogram::style() const
{
	return style_;
}

void Stereogram::setStyle(Stereogram::Style s)
{
	style_ = s;
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

std::tr1::shared_ptr<Texture> Stereogram::texDepth() const
{
	return texDepth_;
}

std::tr1::shared_ptr<Texture> Stereogram::texLeft() const
{
	return texLeft_;
}

std::tr1::shared_ptr<Texture> Stereogram::texRight() const
{
	return texRight_;
}

void Stereogram::setTexDepth(std::tr1::shared_ptr<Texture> t)
{
	texDepth_ = t;
	if (texDepth_)
		texDepth_->setZoom(zoom());
	recreateStereogram();
}

void Stereogram::setTexLeft(std::tr1::shared_ptr<Texture> t)
{
	texLeft_ = t;
	if (texLeft_)
		texLeft_->setZoom(zoom());
}

void Stereogram::setTexRight(std::tr1::shared_ptr<Texture> t)
{
	texRight_ = t;
	if (texRight_)
		texRight_->setZoom(zoom());
}

std::tr1::shared_ptr<Parameterdialog> Stereogram::createDialog()
{
	return std::tr1::shared_ptr<Parameterdialog>(new ParameterStereogram(sharedPtr()));
}

// Serialisation
std::string Stereogram::className() const
{
	return "Stereogram";
}

std::string Stereogram::toLua(std::ostream & outStream) const
{
	// Let parent create an instance
	std::string name = AbstractTexture::toLua(outStream);
	// Set parameters
	outStream << name << ":" << "setOffset(" << offset() << ");\n";
	outStream << name << ":" << "setStyle(\"";
	switch (style())
	{
		case convex:
			outStream << "convex";
			break;
		case concave:
			outStream << "concave";
			break;
	}
	outStream << "\");\n";
	return name;
}

std::string Stereogram::toLuaCreate(std::ostream & outStream) const
{
	std::string name = "tex";
	outStream << name << " = " << className() << "(\""
		<< texLeft()->path() << "\", \"" << texRight()->path() << "\");\n";
	return name;
}

}
