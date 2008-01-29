/*
 *  texture.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "texture.h"

#include "glwidget.h"
#include <QImage>

namespace Examination
{

Texture::Texture(const char * path)
{
	image_ = new QImage(path);

	glTexID_ = 0;
}

Texture::Texture(std::string path)
{
	image_ = new QImage(path.c_str());
	
	glTexID_ = 0;
}

Texture::Texture(QImage* image)
{
	image_ = image;

	glTexID_ = 0;
}

Texture::~Texture()
{
	deleteGlTexID();
	delete image_;
}

void Texture::glBindTex(GLWidget * w)
{
	glBindTexture(GL_TEXTURE_2D, glTexID(w));
}

unsigned int Texture::glTexID(GLWidget * w)
{
	if (w != glWidget_)
		deleteGlTexID();
	
	if (glTexID_)
	{
		return glTexID_;
	}
	else
	{
		glTexID_ = w->bindTexture(*image_);
		glWidget_ = w;
		
		glBindTexture(GL_TEXTURE_2D, glTexID_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		return glTexID_;
	}
}

void Texture::deleteGlTexID()
{
	if (glTexID_)
	{
		glWidget_->deleteTexture(glTexID_);
	}
}

QImage * Texture::image()
{
	return image_;
}

bool Texture::valid()
{
	return image_ && (!image_->isNull());
}
	
// STATIC
Texture * Texture::loadImageFromFile(std::string path)
{
//	TextCache::iterator f = pathToTexture_.find(path);
//	if (f == pathToTexture_.end())
//	{
//		return (*f).second;
//	}
//	else
//	{
		Texture * t = new Texture(path);
		if (t->valid())
		{
			//pathToTexture_[path] = t;
			return t;
		}
		else
		{
			delete t;
			return 0;
		}
//	}
//	return 0;
}

std::map<std::string, Texture*> Texture::pathToTexture_;

}