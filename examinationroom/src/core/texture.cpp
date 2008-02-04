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
	original_ = new QImage(*image_);
}

Texture::Texture(std::string path)
{
	image_ = new QImage(path.c_str());
	original_ = new QImage(*image_);
}

Texture::Texture(QImage* image)
{
	image_ = image;
	original_ = new QImage(*image_);
}

Texture::~Texture()
{
	delete image_;
	delete original_;
}

void Texture::glBindTex(GLWidget * w)
{
	w->bindTexture(*image_); // SLOW?! Only without shared contexts.

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

QImage * Texture::image()
{
	return image_;
}

bool Texture::valid()
{
	return image_ && (!image_->isNull());
}

void Texture::resizeTo(int width, int height)
{
	delete image_;
	image_ = new QImage(original_->scaled(width, height));
}

void Texture::resizeToOriginal()
{
	delete image_;
	image_ = new QImage(*original_);
}

int Texture::width()
{
	return image()->width();
}

int Texture::height()
{
	return image()->height();
}

}