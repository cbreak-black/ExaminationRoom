/*
 *  logline.h
 *  StatisticsRoom
 *
 *  Created by CBreak on 29.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef LOGLINE_H
#define LOGLINEL_H

#include <QDateTime>
#include <QString>

namespace Statistics
{

class LogLine
{
public:
	LogLine(QDateTime timestamp, QString message);

private:
	QDateTime	timestamp_;
	QString		message_;
};

}
#endif