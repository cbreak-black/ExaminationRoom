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

#include <iostream>

namespace Examination
{

Texture::Texture(const char * path)
{
	image_ = QImage(path);
	original_ = image_;
	
	imageGLID_ = 0;
}

Texture::Texture(std::string path)
{
	image_ = QImage(path.c_str());
	original_ = image_;
	
	imageGLID_ = 0;
}

Texture::Texture(QImage image)
{
	image_ = image;
	original_ = image;
	
	imageGLID_ = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &imageGLID_);
	// Delete associated GL Textures
}

void Texture::glBindTex(GLWidget * w)
{
//	imageGLID_ = w->bindTexture(image_); // SLOW?! Only without shared contexts.
	w->makeCurrent();
	if (imageGLID_ == 0)
	{
		glGenTextures(1, &imageGLID_);
		glBindTexture(GL_TEXTURE_2D,imageGLID_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

#ifdef DEBUG
		int er = 0;
#endif
		
		if (image_.format() == QImage::Format_Indexed8)
		{
#ifdef DEBUG
			er = glGetError(); // Clean errors
#endif
			GLubyte * t =  image_.bits();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image_.width(), image_.height(), 0, GL_COLOR_INDEX,
						 GL_UNSIGNED_BYTE, t);
#ifdef DEBUG
			er = glGetError();
#endif
		}
		else
		{
			QImage tx = image_.convertToFormat(QImage::Format_ARGB32);
			if (QSysInfo::ByteOrder == QSysInfo::BigEndian)
			{
				// mirror + swizzle
				QImage tm(tx.size(), QImage::Format_ARGB32);
				for (int i=0; i < tx.height(); i++) {
					uint *p = (uint*) tx.scanLine(i);
					uint *q = (uint*) tm.scanLine(tx.height() - i - 1);
					uint *end = p + tx.width();
					while (p < end)
					{	// To RGBA
						*q = ((*p << 8) & 0xff000000)	// Red
						| ((*p << 8) & 0x00ff0000)	// Green
						| ((*p << 8) & 0x0000ff00)	// Blue
						| ((*p >> 24) & 0x000000ff);	// Alpha
						p++;
						q++;
					}
				}
				tx = tm;
			}
			else
			{
				QImage tm(tx.size(), QImage::Format_ARGB32);
				for (int i=0; i < tx.height(); i++) {
					uint *p = (uint*) tx.scanLine(i);
					uint *q = (uint*) tm.scanLine(tx.height() - i - 1);
					uint *end = p + tx.width();
					while (p < end)
					{       // To RGBA
						*q = ((*p << 0) & 0x000000ff)   // Red
						| ((*p << 0) & 0x0000ff00)      // Green
						| ((*p << 0) & 0x00ff0000)      // Blue
						| ((*p >> 0) & 0xff000000);    // Alpha
						p++;
						q++;
					}
				}
				tx = tm;
			}
#ifdef DEBUG
			er = glGetError(); // Clean errors
#endif
			GLubyte * t =  tx.bits();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tx.width(), tx.height(), 0, GL_RGBA,
						 GL_UNSIGNED_BYTE, t);
#ifdef DEBUG
			er = glGetError();
#endif
		}

#ifdef DEBUG
		if (er == GL_INVALID_VALUE)
		{
			// Only Power of Two textures
			std::cerr << "On this system, only power of two textures are supported\n";
		}
#endif
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D,imageGLID_);
	}
}

void Texture::draw(GLWidget * w)
{
	w->makeCurrent();
	if (image_.format() == QImage::Format_Indexed8)
	{
		GLubyte * t =  image_.bits();
		glDrawPixels(image_.width(), image_.height(), GL_COLOR_INDEX, GL_UNSIGNED_BYTE, t);
	}
	else
	{
		QImage tx = image_.convertToFormat(QImage::Format_ARGB32);
		if (QSysInfo::ByteOrder == QSysInfo::BigEndian)
		{
			// mirror + swizzle
			QImage tm(tx.size(), QImage::Format_ARGB32);
			for (int i=0; i < tx.height(); i++) {
				uint *p = (uint*) tx.scanLine(i);
				uint *q = (uint*) tm.scanLine(tx.height() - i - 1);
				uint *end = p + tx.width();
				while (p < end)
				{	// To RGBA
					*q = ((*p << 8) & 0xff000000)	// Red
					| ((*p << 8) & 0x00ff0000)	// Green
					| ((*p << 8) & 0x0000ff00)	// Blue
					| ((*p >> 24) & 0x000000ff);	// Alpha
					p++;
					q++;
				}
			}
			tx = tm;
		}
		else
		{
			QImage tm(tx.size(), QImage::Format_ARGB32);
			for (int i=0; i < tx.height(); i++) {
				uint *p = (uint*) tx.scanLine(i);
				uint *q = (uint*) tm.scanLine(tx.height() - i - 1);
				uint *end = p + tx.width();
				while (p < end)
				{       // To RGBA
					*q = ((*p << 0) & 0x000000ff)   // Red
					| ((*p << 0) & 0x0000ff00)      // Green
					| ((*p << 0) & 0x00ff0000)      // Blue
					| ((*p >> 0) & 0xff000000);    // Alpha
					p++;
					q++;
				}
			}
			tx = tm;
		}
		GLubyte * t =  tx.bits();
		glDrawPixels(tx.width(), tx.height(), GL_RGBA, GL_UNSIGNED_BYTE, t);
	}
}

QImage Texture::image()
{
	return image_;
}

bool Texture::valid()
{
	return image_.isNull();
}

void Texture::resizeTo(int width, int height)
{
	glDeleteTextures(1, &imageGLID_); // Delete old image
	imageGLID_ = 0;
	image_ = original_.scaled(width, height);
}

void Texture::resizeToOriginal()
{
	glDeleteTextures(1, &imageGLID_); // Delete old image
	imageGLID_ = 0;
	image_ = original_;
}

int Texture::width()
{
	return image().width();
}

int Texture::height()
{
	return image().height();
}

}
