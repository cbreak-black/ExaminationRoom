/*
 *  stereogram.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef STEREOGRAM_H
#define STEREOGRAM_H

#include <memory>

#include "abstracttexture.h"

namespace Examination
{
	class Texture;

/**
This is a class for stereogram textures. It contains one texture for the left
camera and one for the right camera.
*/
class Stereogram : public AbstractTexture
{
public: // Enums
	/**
	The style of the stereogram. Convex means going out of the screen, Concave means
	going into the screen.
	*/
	typedef enum { convex, concave } Style;

public: // Constructors and Destructor
	/**
	Creates a new stereogram by taking the passed textures as left and right
	sides.
	 \param l	Left texture
	 \param r	Right texture
	*/
	Stereogram(std::tr1::shared_ptr<Texture> l, std::tr1::shared_ptr<Texture> r);

protected:
	/**
	Creates a new empty stereogram. Should only be used by subclasses.
	*/
	Stereogram();

public:
	/**
	Binds the suitable texture to the passed widget and returns the texture id.
	Can be called repeatedly.
	 \param w	A GLWidget
	*/
	virtual void glBindTex(GLWidget * w);

	/**
	Draws the texture at the current raster pos.
	 \param w	A GLWidget
	*/
	virtual void draw(GLWidget * w);

public: // Resizing
	virtual void resizeTo(int width, int height);
	virtual void resizeToOriginal();
	virtual int width();
	virtual int height();
	/**
	Returns the zoom factor. Zooming does not change the internal size of the depth texture,
	but the drawing resolution of the stereogram. When drawing it directly, it wil be coarser
	resolved. The sterograms will be recreated to fit. Mapping as texture is not influenced.
	Default is 1.0f;
	 \return the zoom factor
	*/
	virtual float zoomX();
	virtual float zoomY();
	virtual void setZoom(float zx, float zy);

public:
	virtual int offset();
	virtual void setOffset(int o);
	virtual Style style();
	virtual void setStyle(Style s);

private:
	float zoomFactorX_;
	float zoomFactorY_;
	int offset_;	/**< Pixel offset between left and right */
	Style style_;

private:
	virtual void recreateStereogram();

protected:
	std::tr1::shared_ptr<Texture> texDepth();
	std::tr1::shared_ptr<Texture> texLeft();
	std::tr1::shared_ptr<Texture> texRight();
	void setTexDepth(std::tr1::shared_ptr<Texture> t);
	void setTexLeft(std::tr1::shared_ptr<Texture> t);
	void setTexRight(std::tr1::shared_ptr<Texture> t);

private: // Subtextures
	std::tr1::shared_ptr<Texture> texDepth_;
	std::tr1::shared_ptr<Texture> texLeft_;
	std::tr1::shared_ptr<Texture> texRight_;
};

}

#endif
