/*
 *  stereogram.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef STEREOGRAM_H
#define STEREOGRAM_H

#include "abstracttexture.h"

namespace Examination
{
	class Texture;
	
/**
This is a class for stereogram textures. It contains one texture for the left
camera and one for the right camera. It is able to create random dot stereograms
from a depth map texture.
*/
class Stereogram : public AbstractTexture
{
public: // Constructors and Destructor
	/**
	Creates a new stereogram by taking the passed texture as depth map. This method
	takes ownership of the passed texture and releases it when the instance is
	destroyed.
	 \param d	Depth map
	*/
	Stereogram(Texture* d);

	/**
	Creates a new stereogram by taking the passed textures as left and right
	sides. This method takes ownership of the passed textures, and releases them
	when the stereogram instance is destroyed.
	 \param l	Left texture
	 \param r	Right texture
	*/
	Stereogram(Texture* l, Texture* r);
	
	/**
	Deallocates the stereogram and all stored textures.
	*/
	~Stereogram();

public:
	/**
	Binds the suitable texture to the passed widget and returns the texture id.
	Can be called repeatedly.
	 \param w	A GLWidget
	*/
	virtual void glBindTex(GLWidget * w);

private:
	Texture * texDepth_;
	Texture * texLeft_;
	Texture * texRight_;
};
	
}

#endif
