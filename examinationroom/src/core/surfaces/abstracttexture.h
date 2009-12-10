/*
 *  abstracttexture.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef ABSTRACTTEXTURE_H
#define ABSTRACTTEXTURE_H

#include "vec.h"

#include <string>
#include <iostream>
#include <memory>

namespace Examination
{
	class GLWidget;
	class Parameterdialog;
	class AbstractTexture;

/**
The type of texture pointers.
*/
typedef std::tr1::shared_ptr<AbstractTexture> AbstractTexturePtr;
typedef std::tr1::shared_ptr<const AbstractTexture> ConstAbstractTexturePtr;

/**
\defgroup Surfaces	Surfaces
Surfaces are various types of textures. They can be mapped on Object instances, or drawn
as raw pixel data with a Pixelplane.
Surfaces can have different pixel data depending on the eye they are looked at with.
*/

/**
This is an abstract base class for textures. It provides an interface
for clients to use all kinds of textures easily.
Always wrap it into shared_ptr, never use it with direct pointers or by-value.
 \ingroup Surfaces
*/
class AbstractTexture : public std::tr1::enable_shared_from_this<AbstractTexture>
{
public: // Enums
	/**
	 Enum for the supported interpolation types
	 */
	typedef enum {
		nearest = 0, /**< No filtering with GL_NEAREST */
		linear = 1 /**< Linear filtering with GL_LINEAR */
	} FilterType;

public:
	AbstractTexture();
	virtual ~AbstractTexture();

public: // Clone
	/**
	Creates a clone of this texture and returns a shared pointer to it.
	 \return a clone of the texture
	*/
	virtual std::tr1::shared_ptr<AbstractTexture> clone() const = 0;

public: // Shared Pointers
	/**
	Returns the shared_ptr to this object.
	This method only works if the object is referenced by a shared_ptr,
	which is required for some functionality.
	If the shared_ptr could not be locked, an invalid shared_ptr is returned.
	To cast it to a subclass, use std::tr1::dynamic_pointer_cast()
	 \return A shared_ptr to this object
	*/
	AbstractTexturePtr sharedPtr();

	/**
	 \overload
	*/
	ConstAbstractTexturePtr sharedPtr() const;

public:
	/**
	Binds the suitable texture.
	Can be called repeatedly.
	This is the way to use it as texture on an object.
	 \param w	A GLWidget
	*/
	virtual void glBindTex(GLWidget * w) = 0;

	/**
	Draws the texture at the current raster pos.
	This is the way to use it as raw pixel data on screen.
	 \param w	A GLWidget
	*/
	virtual void draw(GLWidget * w) = 0;

public: // Resizing
	/**
	Rescales the texture to the requested size.
	 \param width	Intended width of texture in pixel
	 \param height	Intended height of thexture in pixel
	*/
	virtual void resizeTo(int width, int height) = 0;

	/**
	Resizes the texture to the original size.
	This should if possible always use the original image data, and not repeatedly
	scale back images at the cost of quality.
	*/
	virtual void resizeToOriginal() = 0;

	/**
	Returns the texture width.
	 \return the texture width
	*/
	virtual int width() const = 0;

	/**
	Returns the texture height.
	 \return the texture height
	*/
	virtual int height() const = 0;

	/**
	Returns the zoom factor. Zooming does not change the internal size of a texture, but
	it's drawing resolution. When drawing it directly, it wil be coarser resolved. Mapping
	as texture is not influenced. Default is 1.0f;
	 \return the zoom factor as Vec2f
	*/
	virtual Tool::Vec2f zoom() const;

	/**
	Sets the zoom factor.
	Negative values flip the texture.
	This only applies when the texture is drawn as pixel data.
	 \see draw()
	 \param zx	the new zoom factor in x direction
	 \param zy	the new zoom factor in y direction
	*/
	virtual void setZoom(float zx, float zy);

	/**
	Sets the zoom factor.
	Negative values flip the texture.
	This only applies when the texture is drawn as pixel data.
	This method calls setZoom(float, float), overwrite that method if needed.
	 \param z	A Vec2f object containing the x and y zoom factors
	*/
	void setZoom(const Tool::Vec2f & z);

	/**
	Returns the filter type. Filtering is performed by OpenGL on textures to find a suitable
	color value for each pixel on the screen from the texture.
	 \return the filter type
	*/
	AbstractTexture::FilterType filterType() const;

	/**
	Sets the filter type. Filtering is performed by OpenGL on textures to find a suitable
	color value for each pixel on the screen from the texture. This method accepts OpenGL
	constants as well as this class' enum. Anything else does not change the filter type.
	 \param f	Filter type
	*/
	virtual void setFilterType(FilterType f);

protected:
	/**
	Returns the filter type as OpenGL Constant, either GL_NEAREST or GL_LINEAR at the moment
	 \return the filter type as OpenGL Constant.
	*/
	unsigned int filterTypeGL() const;

public: // Parameter Dialog
	/**
	Returns a ParameterDialog subclass instance associated with this object.
	If none is cached, it is created and returned, otherwise a cached instance
	is returned.
	Does not exist in a constant variant, since changing parameters for
	constant objects does not make sense.
	 \return pointer to a ParameterDialog
	*/
	std::tr1::shared_ptr<Parameterdialog> dialog();

	/**
	Creates a parameter dialog and stores it internally.
	Subclasses should overwrite this method to create their own instances.
	*/
	virtual std::tr1::shared_ptr<Parameterdialog> createDialog();

public: // Serialisation
	/**
	Returns the name of the "class" of this texture. This can be used in LUA
	object creation. Note that this is not the C++ class name, but the LUA type name.
	 \return The name of this object's type as c++ string
	*/
	virtual std::string className() const = 0;

	/**
	Writes the LUA commands to set parameters of this texture to the output stream.
	 \param outStream	A stream that accepts writing
	*/
	virtual std::string toLua(std::ostream & outStream) const;

	/**
	Writes the LUA creation command of this texture to the output stream.
	 \param outStream	A stream that accepts writing
	*/
	virtual std::string toLuaCreate(std::ostream & outStream) const = 0;

private:
	float zoomFactorX_;
	float zoomFactorY_;
	FilterType filterType_;

	std::tr1::shared_ptr<Parameterdialog> dialog_;
};

}
#endif
