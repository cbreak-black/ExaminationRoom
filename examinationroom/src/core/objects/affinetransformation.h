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

#include <memory>
#include <set>

namespace Examination
{

/**
This class represents a renderable piece of text.
*/
class AffineTransformation : public Object
{
public: // Construction
	AffineTransformation();

public:
	virtual void draw(GLWidget * dest);

private:
	std::set< std::tr1::shared_ptr<Object> > objects_;
};
	
}

#endif
