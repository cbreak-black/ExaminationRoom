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

#include "glerrortool.h"

#include "platform_math.h"

#include "parameter/parametertexture.h"

namespace Examination
{

Texture::Texture(const char * path)
{
	imageGLID_ = 0;
	setPath(path);
}

Texture::Texture(const std::string & path)
{
	imageGLID_ = 0;
	setPath(path);
}

Texture::Texture(QImage image)
{
	image_ = image;
	original_ = image;

	imageGLID_ = 0;
}

Texture::Texture(const Texture & t)
	: AbstractTexture(t)
{
	image_ = t.image_;
	imagePath_ = t.imagePath_;
	original_ = t.original_;

	imageGLID_ = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &imageGLID_);
	// Delete associated GL Textures
	GlErrorTool::getErrors("Texture::~Texture");
}

std::tr1::shared_ptr<AbstractTexture> Texture::clone() const
{
	return std::tr1::shared_ptr<AbstractTexture>(new Texture(*this));
}

void Texture::loadPixelMap(QImage image)
{
	GlErrorTool::getErrors("Texture::loadPixelMap:1");
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
	GlErrorTool::getErrors("Texture::loadPixelMap:2");
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
	GlErrorTool::getErrors("Texture::glBindTex:1");
	if (imageGLID_ == 0)
	{
		glGenTextures(1, &imageGLID_);
		glBindTexture(GL_TEXTURE_2D,imageGLID_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterTypeGL());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterTypeGL());

		GlErrorTool::getErrors("Texture::glBindTex:2");
		if (image_.format() == QImage::Format_Indexed8)
		{
			loadPixelMap(image_);
			GLubyte * t =  image_.bits();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image_.width(), image_.height(), 0, GL_COLOR_INDEX,
						 GL_UNSIGNED_BYTE, t);
			GlErrorTool::getErrors("Texture::glBindTex:3");
		}
		else
		{
			GLubyte * t =  image_.bits();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_.width(), image_.height(), 0, GL_RGBA,
						 GL_UNSIGNED_BYTE, t);
			GlErrorTool::getErrors("Texture::glBindTex:4");
		}
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D,imageGLID_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterTypeGL());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterTypeGL());
	}
	GlErrorTool::getErrors("Texture::glBindTex:5");
}

void Texture::draw(GLWidget * /* w */)
{
	GlErrorTool::getErrors("Texture::draw:1");
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
		GlErrorTool::getErrors("Texture::draw:2");
	}
	else
	{
		t =  image_.bits();
		z = zoom();
		glPixelZoom(z.x,z.y);
		glDrawPixels(image_.width(), image_.height(), GL_RGBA, GL_UNSIGNED_BYTE, t);
		glPixelZoom(1.0f,1.0f);
		GlErrorTool::getErrors("Texture::draw:3");
	}
}

QImage Texture::image() const
{
	return image_;
}

bool Texture::valid() const
{
	return !image_.isNull();
}

std::string Texture::path() const
{
	return imagePath_;
}

void Texture::setPath(const std::string & path)
{
	glDeleteTextures(1, &imageGLID_); // Delete old image
	image_ = QImage(QString::fromStdString(path));
	// When loading from path, change color
	// and mirror image verticaly
	if (image_.format() == QImage::Format_Indexed8)
	{
		image_ = image_.mirrored(false, true);
	}
	else
	{
		image_ = transformColorSpace(image_);
	}
	imagePath_ = path;
	original_ = image_;

	imageGLID_ = 0;
}

void Texture::resizeTo(int width, int height)
{
	glDeleteTextures(1, &imageGLID_); // Delete old image
	imageGLID_ = 0;
	if (!original_.isNull())
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

std::tr1::shared_ptr<Parameterdialog> Texture::createDialog()
{
	return std::tr1::shared_ptr<Parameterdialog>(new ParameterTexture(sharedPtr()));
}

std::string Texture::className() const
{
	return "Texture";
}

std::string Texture::toLuaCreate(std::ostream & outStream) const
{
	std::string name = "tex";
	outStream << name << " = " << className() << "(\"" << imagePath_ << "\");\n";
	return name;
}

}
