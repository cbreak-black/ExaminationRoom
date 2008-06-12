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
	Texture(const std::string & path);

	/**
	Creates a texture with an internal copy of a passed QImage as texture.
	 \param image	A QImage object containing texture data
	*/
	Texture(QImage image);

	/**
	Creates a clone of texture t.
	*/
	Texture(const Texture & t);

	/**
	Deletes the texture
	*/
	virtual ~Texture();

public: // Clone
	/**
	Creates a clone of this texture and returns a shared pointer to it.
	 \return a clone of the texture
	*/
	virtual std::tr1::shared_ptr<AbstractTexture> clone() const;

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
	QImage image() const;

	/**
	Returns true if the image is valid, false otherwise.
	 \return true if the image is valid, false otherwise.
	*/
	bool valid() const;

	/**
	Returns the path the image was loaded from, or an empty string if it was created
	in an other way.
	 \return path the image was loaded from
	*/
	std::string path() const;

	/**
	Effectively loads a new texture from the given path.
	Check for valid() to verify if it was successfull.
	*/
	void setPath(const std::string & path);

public: // Resizing
	virtual void resizeTo(int width, int height);
	virtual void resizeToOriginal();
	virtual int width() const;
	virtual int height() const;

public: // Parameter Dialog
	virtual std::tr1::shared_ptr<Parameterdialog> createDialog();

public: // Serialisation
	virtual std::string className() const;
//	virtual std::string toLua(std::ostream & outStream) const;
	virtual std::string toLuaCreate(std::ostream & outStream) const;

private:
	void loadPixelMap(QImage image);
	QImage transformColorSpace(QImage image);

private:
	QImage image_;
	QImage original_;

	std::string imagePath_;

	GLuint imageGLID_;
};

}

#endif
