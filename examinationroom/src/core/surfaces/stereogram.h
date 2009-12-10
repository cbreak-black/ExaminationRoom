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

	/**
	Creates a new stereogram by taking the passed strings as paths to left and right
	sides.
	 \param lpath	Path to Left texture
	 \param rpath	Path to Right texture
	 */
	Stereogram(const std::string & lpath, const std::string & rpath);

	/**
	Creates a clone of the stereogram s.
	 \param s	A stereogram
	*/
	Stereogram(const Stereogram & s);

protected:
	/**
	Creates a new empty stereogram. Should only be used by subclasses.
	*/
	Stereogram();

public: // Clone
	/**
	Creates a clone of this texture and returns a shared pointer to it.
	 \return a clone of the texture
	*/
	virtual std::tr1::shared_ptr<AbstractTexture> clone() const;

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
	virtual int width() const;
	virtual int height() const;
	virtual void setZoom(float zx, float zy);
	virtual void setFilterType(AbstractTexture::FilterType f);

public:
	/**
	Returns the stereo offset in pixel. The offset is the horizontal distance from
	the maximal depth value to the minimal. The effective horizontal distance between
	the maximal depth value of the left and right eye is therefore twice the offset.
	The default value is six pixel.
	 \return the current pixel offset value.
	*/
	virtual int offset() const;

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
	virtual Style style() const;

	/**
	Sets the style of the stereogram. The choices are convex and concave
	 \see Style
	 \param s	New stereogram style
	*/
	virtual void setStyle(Style s);

public: // Parameter Dialog
	virtual std::tr1::shared_ptr<Parameterdialog> createDialog();

public: // Serialisation
	virtual std::string className() const;
	virtual std::string toLua(std::ostream & outStream) const;
	virtual std::string toLuaCreate(std::ostream & outStream) const;

private:
	int offset_;	/**< Horizontal pixel offset between max and min depth */
	Style style_;

public:
	/**
	Recreates the stereogram after a resize or other parameter change.
	This function should be reimplemented by subclasses.
	Call this method when the underlying data structure texDepth() was
	modified directly.
	This method is automatically called when the Stereogram itself is modified.
	*/
	virtual void recreateStereogram();

public:
	/**
	The depth map texture is a single texture that contains the depth information
	for stereograms.
	 \return the depth map texture
	*/
	std::tr1::shared_ptr<Texture> texDepth() const;

	/**
	The left texture is the image seen by the left eye.
	Subclasses create their own texture data from the depth map.
	 \return the left eye texture
	*/
	std::tr1::shared_ptr<Texture> texLeft() const;

	/**
	The left texture is the image seen by the right eye.
	Subclasses create their own texture data from the depth map.
	 \return the right eye texture
	*/
	std::tr1::shared_ptr<Texture> texRight() const;

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
