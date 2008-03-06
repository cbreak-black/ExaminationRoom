/*
 *  pixelplane.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 12.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PIXELPLANE_H
#define PIXELPLANE_H

#include <memory>

#include "object.h"

namespace Examination
{
	class AbstractTexture;
	
class Pixelplane : public Object
{
public: // Construction
	Pixelplane();
	~Pixelplane();

public:
	virtual void draw(GLWidget * dest) const;
	
	void setSize(float w, float h);

private:
	float width_;
	float height_;
};
	
}

#endif
