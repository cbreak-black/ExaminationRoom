/*
 *  randomdotstereogram.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 19.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef RANDOMDOT_STEREOGRAM_H
#define RANDOMDOT_STEREOGRAM_H

#include "stereogram.h"

namespace Examination
{

/**
This is a class for stereogram textures. It contains one texture for the left
camera and one for the right camera. It is able to create random dot stereograms
from a depth map texture.
*/
class RandomdotStereogram : public Stereogram
{
public: // Constructors and Destructor
	/**
	Creates a new stereogram by taking the passed texture as depth map.
	 \param d	Depth map
	*/
	RandomdotStereogram(std::tr1::shared_ptr<Texture> d);

private:
	virtual void recreateStereogram();
};
	
}

#endif
