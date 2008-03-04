/*
 *  affinetransformation.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef AFFINE_TRANSFORMATION_H
#define AFFINE_TRANSFORMATION_H

#include "object.h"
#include "container.h"

namespace Examination
{

/**
This class represents a transformable group of objects.
*/
class AffineTransformation : public Object, public Container
{
public: // Construction
	AffineTransformation();
	
public:
	virtual void draw(GLWidget * dest);
};
	
}

#endif
