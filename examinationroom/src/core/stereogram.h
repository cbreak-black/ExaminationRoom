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
public: // Constructors and Destructor
	/**
	Creates a new stereogram by taking the passed textures as left and right
	sides.
	 \param l	Left texture
	 \param r	Right texture
	*/
	Stereogram(std::tr1::shared_ptr<Texture> l, std::tr1::shared_ptr<Texture> r);
	
	/**
	Deallocates the stereogram and all stored textures.
	*/
	virtual ~Stereogram();

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

private:
	virtual void recreateStereogram();

protected:
	std::tr1::shared_ptr<Texture> texDepth();
	std::tr1::shared_ptr<Texture> texLeft();
	std::tr1::shared_ptr<Texture> texRight();
	void setTexDepth(std::tr1::shared_ptr<Texture> t);
	void setTexLeft(std::tr1::shared_ptr<Texture> t);
	void setTexRight(std::tr1::shared_ptr<Texture> t);

private:
	std::tr1::shared_ptr<Texture> texDepth_;
	std::tr1::shared_ptr<Texture> texLeft_;
	std::tr1::shared_ptr<Texture> texRight_;
};
	
}

#endif
