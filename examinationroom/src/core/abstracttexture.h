/*
 *  abstracttexture.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 22.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ABSTRACTTEXTURE_H
#define ABSTRACTTEXTURE_H

namespace Examination
{
	class GLWidget;

/**
This is an abstract base class for textures. It provides an interface
for clients to use all kinds of textures easily.
*/
class AbstractTexture
{
public:
	virtual ~AbstractTexture();
public:
	/**
	Binds the suitable texture.
	Can be called repeatedly.
	 \param w	A GLWidget
	*/
	virtual void glBindTex(GLWidget * w) = 0;
};

}
#endif
