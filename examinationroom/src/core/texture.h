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

#include <map>
#include <memory>

#include <QImage>

namespace Examination
{
	class Texture;
	
	typedef std::map<std::string, Texture*> TextCache;
	
/**
This is a class for textures. It contains static factory methods to load image
files from disc.
*/
class Texture : public AbstractTexture
{
public: // Constructors and Destructor
	Texture(const char * path);
	Texture(std::string path);
	Texture(QImage image);
	~Texture();

public:
	/**
	Binds the suitable texture.
	Can be called repeatedly.
	 \param w	A GLWidget
	*/
	virtual void glBindTex(GLWidget * w);

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

private:
	QImage image_;
	QImage original_;
};

}

#endif
