/*
 *  texture.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "texture.h"

#include <qgl.h>

#include "platform_math.h"

namespace Examination
{

Texture::Texture(const char * path)
{
	image_ = QImage(path);
	imagePath_ = path;
	original_ = image_;
	
	imageGLID_ = 0;
}

Texture::Texture(const std::string & path)
{
	image_ = QImage(path.c_str());
	imagePath_ = path;
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

void Texture::loadPixelMap(QImage image)
{
	int nc = image.numColors();
	if (nc > 0)
	{
		int nm = pow(2, ceil(log2(nc)));
		GLfloat * mapR = new GLfloat[nm];
		GLfloat * mapG = new GLfloat[nm];
		GLfloat * mapB = new GLfloat[nm];
		GLfloat * mapA = new GLfloat[nm];
		for (int i = 0; i<nc; i++)
		{
			QRgb c = image.color(i);
			mapR[i] = (float)qRed(c)/0xff;
			mapG[i] = (float)qGreen(c)/0xff;
			mapB[i] = (float)qBlue(c)/0xff;
			mapA[i] = (float)qAlpha(c)/0xff;
		}
		for (int i = nc; i<nm; i++)
		{
			mapR[i] = 0.0f;
			mapG[i] = 0.0f;
			mapB[i] = 0.0f;
			mapA[i] = 0.0f;
		}
		glPixelMapfv(GL_PIXEL_MAP_I_TO_R, nm,mapR);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_G, nm,mapG);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_B, nm,mapB);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_A, nm,mapA);
		delete [] mapR;
		delete [] mapG;
		delete [] mapB;
		delete [] mapA;
	}
}

QImage Texture::transformColorSpace(QImage image)
{
	QImage tx = image.convertToFormat(QImage::Format_ARGB32);
	QImage tm(tx.size(), QImage::Format_ARGB32);
	if (QSysInfo::ByteOrder == QSysInfo::BigEndian)
	{
		// mirror + swizzle
		for (int i=0; i < tx.height(); i++)
		{
			uint *p = (uint*) tx.scanLine(i);
			uint *q = (uint*) tm.scanLine(tx.height() - i - 1);
			uint *end = p + tx.width();
			while (p < end)
			{	// To RGBA
				*q = ((*p << 8) & 0xffffff00)	// Red & Green & Blue
				| ((*p >> 24) & 0x000000ff);	// Alpha
				p++; q++;
			}
		}
	}
	else
	{
		for (int i=0; i < tx.height(); i++)
		{
			uint *p = (uint*) tx.scanLine(i);
			uint *q = (uint*) tm.scanLine(tx.height() - i - 1);
			uint *end = p + tx.width();
			while (p < end)
			{	// To RGBA Little Endian (ABGR)
				*q = ((*p) & 0xff00ff00)		// Green & Alpha
				| ((*p >> 16) & 0x000000ff)		// Red
				| ((*p << 16) & 0x00ff0000);	// Blue
				p++; q++;
			}
		}
	}
	return tm;
}

/**
If the texture is not bound, create a new texture ID and load the pixel data into
GPU memory. Also set up parameters so that the texture repeats and is scaled with
Linear filter.
If it is already loaded into memory, just bind the texture.
 \param w	The GLWidget to draw into
*/
void Texture::glBindTex(GLWidget * /* w */)
{
	if (imageGLID_ == 0)
	{
		glGenTextures(1, &imageGLID_);
		glBindTexture(GL_TEXTURE_2D,imageGLID_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

#ifdef DEBUG
		int er = 0;
#endif
		
		if (image_.format() == QImage::Format_Indexed8)
		{
#ifdef DEBUG
			er = glGetError(); // Clean errors
#endif
			loadPixelMap(image_);
			GLubyte * t =  image_.bits();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image_.width(), image_.height(), 0, GL_COLOR_INDEX,
						 GL_UNSIGNED_BYTE, t);
#ifdef DEBUG
			er = glGetError();
#endif
		}
		else
		{
			QImage tx = transformColorSpace(image_);
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

void Texture::draw(GLWidget * /* w */)
{
	GLubyte * t;
	Tool::Vec2f z;
	if (image_.format() == QImage::Format_Indexed8)
	{
		loadPixelMap(image_);
		t =  image_.bits();
		z = zoom();
		glPixelZoom(z.x,z.y);
		glDrawPixels(image_.width(), image_.height(), GL_COLOR_INDEX, GL_UNSIGNED_BYTE, t);
		glPixelZoom(1.0f,1.0f);
	}
	else
	{
		QImage tx = transformColorSpace(image_);
		t =  tx.bits();
		z = zoom();
		glPixelZoom(z.x,z.y);
		glDrawPixels(tx.width(), tx.height(), GL_RGBA, GL_UNSIGNED_BYTE, t);
		glPixelZoom(1.0f,1.0f);
	}
}

QImage Texture::image() const
{
	return image_;
}

bool Texture::valid() const
{
	return image_.isNull();
}

std::string Texture::path() const
{
	return imagePath_;
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

int Texture::width() const
{
	return image().width();
}

int Texture::height() const
{
	return image().height();
}

std::string Texture::className() const
{
	return "Texture";
}

std::string Texture::toLua(std::ostream & outStream) const
{
	std::string name = toLuaCreate(outStream);
	return name;
}

/**
 \todo	Remove evil global variable name usage once the program/namemanager are implemented
*/
std::string Texture::toLuaCreate(std::ostream & outStream) const
{
	std::string name = "tex";
	outStream << name << " = " << className() << "(\"" << imagePath_ << "\");\n";
	return name;
}

}
