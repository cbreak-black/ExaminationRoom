/*
 *  platform_string.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 24.04.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PLATFORM_STRING_H
#define PLATFORM_STRING_H

#include "platform.h"

#ifdef WINDOWS

#define snprintf _snprintf_s

#endif

#endif