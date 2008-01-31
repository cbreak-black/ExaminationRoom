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
}

Texture::Texture(std::string path)
{
	image_ = new QImage(path.c_str());
}

Texture::Texture(QImage* image)
{
	image_ = image;
}

Texture::~Texture()
{
	deleteGlTexIDs();
	delete image_;
}

void Texture::glBindTex(GLWidget * w)
{
	//glBindTexture(GL_TEXTURE_2D, glTexID(w));
	w->bindTexture(*image_); // SLOW?!
}

unsigned int Texture::glTexID(GLWidget * w)
{
	unsigned int id = widgetToID_[w];
	
	if (id)
	{
		return id;
	}
	else
	{
		id = w->bindTexture(*image_);
		widgetToID_[w] = id;

		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		return id;
	}
}

void Texture::deleteGlTexIDs()
{
	std::map<GLWidget*, unsigned int>::iterator i = widgetToID_.begin();
	for (; i != widgetToID_.end(); i++)
	{
		i->first->deleteTexture(i->second);
	}
	widgetToID_.clear();
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