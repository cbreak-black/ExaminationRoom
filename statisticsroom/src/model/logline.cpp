/*
 *  logline.cpp
 *  StatisticsRoom
 *
 *  Created by CBreak on 29.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "logline.h"

namespace Statistics
{

LogLine::LogLine(QDateTime timestamp, QString message)
{
	timestamp_ = timestamp;
	message_ = message;
}
	
}
