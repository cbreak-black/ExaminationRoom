/*
 *  affinetransformation.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "affinetransformation.h"

#include "vec.h"

using namespace std::tr1;

namespace Examination
{
	using namespace Tool;
	
AffineTransformation::AffineTransformation()
{
}

// Drawing
void AffineTransformation::draw(GLWidget * dest)
{
	Container::draw(dest);
}


}
