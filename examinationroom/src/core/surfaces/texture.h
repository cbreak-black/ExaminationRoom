/*
 *  texture.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "abstracttexture.h"

#include <QImage>
#include <qgl.h>

namespace Examination
{

/**
This is a class for textures.
It can load it's image data from a file on disk (via the QImage class).
While OpenGL originally required power-of-two textures, this restriction was relaxed
with the use of Extensions and finally with the newest OpenGL release, it might or
might not work.
*/
class Texture : public AbstractTexture
{
public: // Constructors and Destructor
	/**
	Loads an image from a path to use as texture.
	Since this uses QImage, only formats supported by it can be loaded.
	 \see QImage
	 \param path	C String to a file containing image data
	*/
	Texture(const char * path);

	/**
	Loads an image from a path to use as texture.
	Since this uses QImage, only formats supported by it can be loaded.
	 \see QImage
	 \param path	STD String to a file containing image data
	*/
	Texture(std::string path);
	/**
	Creates a texture with an internal copy of a passed QImage as texture.
	 \param image	A QImage object containing texture data
	*/
	Texture(QImage image);
	~Texture();

public:
	/**
	Binds the suitable texture.
	Can be called repeatedly.
	 \param w	A GLWidget
	*/
	virtual void glBindTex(GLWidget * w);

	/**
	Draws the texture at the current raster pos.
	 \param w	A GLWidget
	*/
	virtual void draw(GLWidget * w);

public:
	/**
	Returns the stored image that is used as texture.
	 \return the stored image that is used as texture.
	*/
	QImage image();

	/**
	Returns true if the image is valid, false otherwise.
	 \return true if the image is valid, false otherwise.
	*/
	bool valid();

public: // Resizing
	virtual void resizeTo(int width, int height);
	virtual void resizeToOriginal();
	virtual int width();
	virtual int height();
	virtual float zoomX();
	virtual float zoomY();
	virtual void setZoom(float zx, float zy);

private:
	void loadPixelMap(QImage image);
	QImage transformColorSpace(QImage image);

private:
	QImage image_;
	QImage original_;

	GLuint imageGLID_;
	float zoomFactorX_;
	float zoomFactorY_;
};

}

#endif
