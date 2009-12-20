/*
 *  logline.cpp
 *  StatisticsRoom
 *
 *  Created by CBreak on 29.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "logline.h"

#include <QStringList>

using namespace std::tr1;

namespace Statistics
{

static const char * dateTimeFormatString = "yyyy.MM.dd hh:mm:ss.zzz";

LogLine::LogLine(QDateTime timestamp, QString message)
{
	timestamp_ = timestamp;
	message_ = message;
}

bool LogLine::isValid() const
{
	return timestamp_.isValid();
}

QDateTime LogLine::timestamp() const
{
	return timestamp_;
}

QString LogLine::message() const
{
	return message_;
}

shared_ptr<LogLine> LogLine::logLineFromString(QString str)
{
	QStringList sl = str.split(": [LOG][Lua] ");
	if (sl.size() == 2)
	{
		QDateTime dt = QDateTime::fromString(sl.at(0), dateTimeFormatString);
		if (dt.isValid())
			return shared_ptr<LogLine>(new LogLine(dt, sl.at(1)));
	}
	// Invalid
	return shared_ptr<LogLine>(new LogLine(QDateTime(), str));
}

}
