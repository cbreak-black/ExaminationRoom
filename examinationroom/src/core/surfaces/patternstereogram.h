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
	/**
	Creates a new stereogram by taking the passed strings as paths to textures for
	patterns and depth maps.
	 \param d	Depth map path
	 \param b	Background pattern path
	 \param f	Foreground pattern path
	*/
	PatternStereogram(const std::string & d, const std::string & b, const std::string & f);

	/**
	Creates a clone of a pattern stereogram.
	*/
	PatternStereogram(const PatternStereogram & p);

public: // Clone
	/**
	Creates a clone of this texture and returns a shared pointer to it.
	 \return a clone of the texture
	*/
	virtual std::tr1::shared_ptr<AbstractTexture> clone() const;

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
