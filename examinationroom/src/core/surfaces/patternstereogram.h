/*
 *  patternstereogram.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 19.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PATTERN_STEREOGRAM_H
#define PATTERN_STEREOGRAM_H

#include <memory>

#include "stereogram.h"

namespace Examination
{

/**
This is a class for stereogram textures. It contains one texture for the left
camera and one for the right camera. It is able to create stereograms
from a depth map texture and a source pattern.
 \ingroup Surfaces
*/
class PatternStereogram : public Stereogram
{
public: // Constructors and Destructor
	/**
	Creates a new stereogram by taking the passed texture as depth map.
	 \param d	Depth map
	 \param p	Background pattern map
	*/
	PatternStereogram(std::tr1::shared_ptr<Texture> d, std::tr1::shared_ptr<Texture> p);
	/**
	 Creates a new stereogram by taking the passed texture as depth map.
	 \param d	Depth map
	 \param p	Background pattern map
	 \param q	Foreground pattern map
	 */
	PatternStereogram(std::tr1::shared_ptr<Texture> d, std::tr1::shared_ptr<Texture> p, std::tr1::shared_ptr<Texture> q);

public: // Serialisation
	virtual std::string className() const;
	virtual std::string toLua(std::ostream & outStream) const;
	virtual std::string toLuaCreate(std::ostream & outStream) const;

private:
	virtual void recreateStereogram();

private:
	std::tr1::shared_ptr<Texture> bgPattern_;
	std::tr1::shared_ptr<Texture> fgPattern_;
};
	
}

#endif
