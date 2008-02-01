/*
 *  platform_math.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 01.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PLATFORM_MATH_H
#define PLATFORM_MATH_H

#include "platform.h"

#include <math.h>

#ifdef WINDOWS
	#define roundf(f)	floorf(f+0.5)
#endif

#endif
