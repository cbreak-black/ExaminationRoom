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
 \ingroup Surfaces
*/
class Stereogram : public AbstractTexture
{
public: // Enums
	/**
	The style of the stereogram. Convex means going out of the screen, Concave means
	going into the screen.
	*/
	typedef enum {
		convex, /**< The depth map maps black to zero paralax and white to comming out of screen */
		concave /**< The depth map maps black to zero paralax and white to going into the screen */
	} Style;

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

public: // Resizing (Implemented from parent)
	virtual void resizeTo(int width, int height);
	virtual void resizeToOriginal();
	virtual int width();
	virtual int height();
	virtual float zoomX();
	virtual float zoomY();
	virtual void setZoom(float zx, float zy);

public:
	/**
	Returns the stereo offset in pixel. The offset is the horizontal distance from
	the maximal depth value to the minimal. The effective horizontal distance between
	the maximal depth value of the left and right eye is therefore twice the offset.
	The default value is six pixel.
	 \return the current pixel offset value.
	*/
	virtual int offset();

	/**
	Sets the offset value.
	 \see offset()
	 \param o	New offset value in pixel. Negative values cause potentially undesired results.
	*/
	virtual void setOffset(int o);

	/**
	Returns the style of the stereogram. For more information on styles, see the thesis.
	The default is convex.
	 \return the current stereogram style.
	*/
	virtual Style style();

	/**
	Sets the style of the stereogram. The choices are convex and concave
	 \see Style
	 \param s	New stereogram style
	*/
	virtual void setStyle(Style s);

private:
	float zoomFactorX_;
	float zoomFactorY_;
	int offset_;	/**< Horizontal pixel offset between max and min depth */
	Style style_;

private:
	/**
	Recreates the stereogram after a resize or other parameter change.
	This function should be reimplemented by subclasses.
	*/
	virtual void recreateStereogram();

protected:
	/**
	The depth map texture is a single texture that containds the depth information
	for stereograms.
	 \return the depth map texture
	*/
	std::tr1::shared_ptr<Texture> texDepth();

	/**
	The left texture is the image seen by the left eye.
	Subclasses create their own texture data from the depth map.
	 \return the left eye texture
	*/
	std::tr1::shared_ptr<Texture> texLeft();

	/**
	The left texture is the image seen by the right eye.
	Subclasses create their own texture data from the depth map.
	 \return the right eye texture
	*/
	std::tr1::shared_ptr<Texture> texRight();

	/**
	Set the depth map texture.
	 \param t	Texture with depth data
	*/
	void setTexDepth(std::tr1::shared_ptr<Texture> t);

	/**
	Set the left eye texture.
	 \param t	Texture with image data for the right eye
	*/
	void setTexLeft(std::tr1::shared_ptr<Texture> t);
	/**
	Set the right eye texture.
	 \param t	Texture with image data for the left eye
	*/
	void setTexRight(std::tr1::shared_ptr<Texture> t);

private: // Subtextures
	std::tr1::shared_ptr<Texture> texDepth_;
	std::tr1::shared_ptr<Texture> texLeft_;
	std::tr1::shared_ptr<Texture> texRight_;
};

}

#endif
