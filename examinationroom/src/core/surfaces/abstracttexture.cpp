/*
 *  abstracttexture.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "abstracttexture.h"

#include "parameter/parameterabstracttexture.h"

#include "qgl.h" // For constants

namespace Examination
{

AbstractTexture::AbstractTexture()
{
	zoomFactorX_ = 1.0f;
	zoomFactorY_ = 1.0f;
	filterType_ = AbstractTexture::linear;
}

AbstractTexture::~AbstractTexture()
{
}

AbstractTexturePtr AbstractTexture::sharedPtr()
{
	try
	{
		return shared_from_this();
	}
	catch (std::tr1::bad_weak_ptr const & e)
	{
		// No pointer exists yet, return an empty one
		return AbstractTexturePtr();
	}
}

ConstAbstractTexturePtr AbstractTexture::sharedPtr() const
{
	try
	{
		return shared_from_this();
	}
	catch (std::tr1::bad_weak_ptr const & e)
	{
		// No pointer exists yet, return an empty one
		return ConstAbstractTexturePtr();
	}
}

Tool::Vec2f AbstractTexture::zoom() const
{
	return Tool::Vec2f(zoomFactorX_, zoomFactorY_);
}

void AbstractTexture::setZoom(float zx, float zy)
{
	zoomFactorX_ = zx;
	zoomFactorY_ = zy;
}

void AbstractTexture::setZoom(const Tool::Vec2f & z)
{
	// Call main method, which can be overwritten because it's virtual
	setZoom(z.x, z.y);
}

AbstractTexture::FilterType AbstractTexture::filterType() const
{
	return filterType_;
}

unsigned int AbstractTexture::filterTypeGL() const
{
	// Might work more elegant with filterType_+GL_NEAREST, but this is safer
	switch (filterType_)
	{
		case AbstractTexture::nearest:
			return GL_NEAREST;
		case AbstractTexture::linear:
			return GL_LINEAR;
	}
	// Error
	return 0;
}

void AbstractTexture::setFilterType(FilterType f)
{
	switch (f)
	{
		case AbstractTexture::nearest:
		case GL_NEAREST:
			filterType_ = AbstractTexture::nearest;
			break;
		case AbstractTexture::linear:
		case GL_LINEAR:
			filterType_ = AbstractTexture::linear;
			break;
		default:
			break;
	}
}

std::string AbstractTexture::toLua(std::ostream & outStream) const
{
	std::string name = toLuaCreate(outStream);
	Tool::Vec2f z = zoom();
	outStream << name << ":" << "setZoom(" << z.x << ", " << z.y << ");\n";
	outStream << name << ":" << "setFilterType(\"";
	switch (filterType())
	{
		case nearest:
			outStream << "nearest";
			break;
		case linear:
			outStream << "linear";
			break;
	}
	outStream << "\");\n";
	return name;
}

// Parameter Dialog
std::tr1::shared_ptr<Parameterdialog> AbstractTexture::dialog()
{
	if (!dialog_)
	{
		dialog_ = createDialog();
	}
	return dialog_;
}

std::tr1::shared_ptr<Parameterdialog> AbstractTexture::createDialog()
{
	return std::tr1::shared_ptr<Parameterdialog>(new ParameterAbstractTexture(sharedPtr()));
}

}