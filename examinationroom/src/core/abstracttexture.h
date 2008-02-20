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

namespace Examination
{
	class GLWidget;

/**
This is an abstract base class for textures. It provides an interface
for clients to use all kinds of textures easily.
*/
class AbstractTexture
{
public:
	virtual ~AbstractTexture();
public:
	/**
	Binds the suitable texture.
	Can be called repeatedly.
	 \param w	A GLWidget
	*/
	virtual void glBindTex(GLWidget * w) = 0;

	/**
	Draws the texture at the current raster pos.
	 \param w	A GLWidget
	*/
	virtual void draw(GLWidget * w) = 0;

public: // Resizing
	virtual void resizeTo(int width, int height) = 0;
	virtual void resizeToOriginal() = 0;
	virtual int width() = 0;
	virtual int height() = 0;
	/**
	Returns the zoom factor. Zooming does not change the internal size of a texture, but
	it's drawing resolution. When drawing it directly, it wil be coarser resolved. Mapping
	as texture is not influenced. Default is 1.0f;
	 \return the zoom factor
	*/
	virtual float zoom() = 0;
	/**
	Sets the zoom factor.
	 \see zoom()
	 \param z	the new zoom factor
	*/
	virtual void setZoom(float z) = 0;
};

}
#endif
