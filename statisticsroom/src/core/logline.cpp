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
#include <QDebug>

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
	QString strData = str.left(23);
	QString strMsg;
	if (str.contains(": [LOG][Lua] "))
		strMsg = str.mid(23+13);
	else
		strMsg = str.mid(23+2);
	QDateTime dt = QDateTime::fromString(strData, dateTimeFormatString);
	if (dt.isValid())
		return shared_ptr<LogLine>(new LogLine(dt, strMsg));
	// Invalid
	return shared_ptr<LogLine>(new LogLine(QDateTime(), str));
}

}
