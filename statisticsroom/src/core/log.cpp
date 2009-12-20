/*
 *  log.cpp
 *  StatisticsRoom
 *
 *  Created by cbreak on 02.06.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "log.h"

#include <QTextStream>

#include "logline.h"

namespace Statistics
{

Log::Log(QTextStream & input)
{
	while (!input.atEnd())
	{
		// This method parses lines with the pattern:
		// "yyyy.MM.dd hh:mm:ss.zzz: : [LOG][Lua] <message>"
		// If the line does not match this pattern (the date can not be extracted)
		// the returned LogLine is invalid and not appended
		std::tr1::shared_ptr<LogLine> l = LogLine::logLineFromString(input.readLine());
		if (l->isValid())
		{
			append(l);
		}
	}
}

std::tr1::shared_ptr<Log> Log::logFromStream(QTextStream & input)
{
	return std::tr1::shared_ptr<Log>(new Log(input));
}

}
