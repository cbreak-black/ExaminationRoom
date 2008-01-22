/*
 *  texture.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <map>

class QImage;

namespace Examination
{
	class Texture;
	class GLWidget;
	
	typedef std::map<std::string, Texture*> TextCache;
	
/**
This is a class for textures. It contains static factory methods to load image
files from disc.
*/
class Texture
{
public: // Constructors and Destructor
	Texture(std::string path);
	Texture(QImage* image);
	~Texture();

public:
	/**
	Binds the texture to the passed widget and returns the texture id. Can be
	called repeatedly.
	 \param w	A GLWidget
	*/
	unsigned int glTexID(GLWidget * w);
private:
	void deleteGlTexID();
	
public:
	/**
	Returns true if the image is valid, false otherwise.
	 \return true if the image is valid, false otherwise.
	*/
	bool valid();
	
private:
	QImage * image_;
	
	unsigned int glTexID_;
	GLWidget * glWidget_;

// STATIC
public: // Factory methods
	static Texture* loadImageFromFile(std::string path);
		
private: // Static members
	static TextCache pathToTexture_;
};
	
}

#endif
