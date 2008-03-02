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

static char * dateTimeFormatString = "yyyy.MM.dd hh:mm:ss.zzz";

LogLine::LogLine()
{
}

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
	QDateTime dt = QDateTime::fromString(str.section(": ", 0, 0), dateTimeFormatString);
	if (dt.isValid())
		return shared_ptr<LogLine>(new LogLine(dt, str.section(": ", 1)));
	else
		return shared_ptr<LogLine>(new LogLine(dt, str));
}

}
