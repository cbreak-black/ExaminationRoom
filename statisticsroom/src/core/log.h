/*
 *  log.h
 *  StatisticsRoom
 *
 *  Created by cbreak on 02.06.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef LOG_H
#define LOG_H

#include <QList>

#include <memory>

class QTextStream;

namespace Statistics
{
	class LogLine;

/**
This class is a representation of the log that was read.
The lines are split into a date and a data part.
*/
class Log : public QList<std::tr1::shared_ptr<LogLine> >
{
public:
	Log(QTextStream & input);

public:
	static std::tr1::shared_ptr<Log> logFromStream(QTextStream & input);

};

}

#endif
