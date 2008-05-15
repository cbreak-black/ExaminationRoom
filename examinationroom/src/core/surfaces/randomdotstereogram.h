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

#include <vector>

namespace Examination
{

/**
This is a class for stereogram textures. It contains one texture for the left
camera and one for the right camera. It is able to create random dot stereograms
from a depth map texture.
 \ingroup Surfaces
*/
class RandomdotStereogram : public Stereogram
{
public: // Constructors and Destructor
	/**
	Creates a new stereogram by taking the passed texture as depth map.
	 \param d	Depth map
	*/
	RandomdotStereogram(std::tr1::shared_ptr<Texture> d);

	/**
	 Creates a new stereogram by taking the passed string as path to a depth map.
	 \param path	Path to a Depth map
	*/
	RandomdotStereogram(const std::string & path);

public:
	void setMaxColor(int mc);
	void setExclusiveColor(int ec);
	void setColor(int idx, char r, char g, char b);
	void setColor(int idx, unsigned int color);
	void resetColor();

public: // Serialisation
	virtual std::string className() const;
	virtual std::string toLua(std::ostream & outStream) const;
	virtual std::string toLuaCreate(std::ostream & outStream) const;

private:
	virtual void recreateStereogram();
	void applyColorPalette();

private:
	int maxColor_;	/**< Number of grayscales in image */
	int excColor_;	/**< Number of colors exclusively to fg/bg */

	std::vector<unsigned int> colors_;
};
	
}

#endif
