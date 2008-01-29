/*
 *  drawing.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */


#ifndef DRAWING_H
#define DRAWING_H

#include <qgl.h>

namespace Examination
{

/**
Indicates for which side the scene should be drawn.
1 corresponds with the left eye
2 corresponds with the right eye
*/	
typedef  enum {left = 1, right = 2} Side;

}
#endif
