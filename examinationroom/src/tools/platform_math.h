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

#ifdef WINDOWS
	#define _USE_MATH_DEFINES
#endif

#include <math.h>

#ifdef WINDOWS
	#define roundf(f)	(floorf(f+0.5))
	#define log2(f)		(log((float)f)/log(2.0))
#endif

#endif
