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

#include <memory>

namespace Statistics
{

class LogLine
{
public:
	LogLine();
	LogLine(QDateTime timestamp, QString message);

public:
	QDateTime timestamp() const;
	QString message() const;

public:
	bool isValid() const;

public: // Factory Methods
	static std::tr1::shared_ptr<LogLine> logLineFromString(QString str);
	
private:
	QDateTime timestamp_;
	QString message_;
};

}
#endif