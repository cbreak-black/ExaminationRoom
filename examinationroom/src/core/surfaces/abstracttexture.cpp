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

namespace Examination
{

AbstractTexture::AbstractTexture()
{
	zoomFactorX_ = 1.0f;
	zoomFactorY_ = 1.0f;
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

std::string AbstractTexture::toLua(std::ostream & outStream) const
{
	std::string name = toLuaCreate(outStream);
	Tool::Vec2f z = zoom();
	outStream << name << ":" << "setZoom(" << z.x << ", " << z.y << ");\n";
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