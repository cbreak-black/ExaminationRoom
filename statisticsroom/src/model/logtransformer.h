/*
 *  logtransformer.h
 *  StatisticsRoom
 *
 *  Created by cbreak on 02.06.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef LOGTRANSFORMER_H
#define LOGTRANSFORMER_H

#include <QList>

#include <memory>

class QTextStream;

namespace Statistics
{
	class Log;
	class Pattern;

/**
This class encapsulates the transformations that produce a tab separated list
from the time stamped log list.
*/
class LogTransformer
{
public:
	LogTransformer();

public: // Statistics&Transformation
	void transformLog(std::tr1::shared_ptr<Log> log, QTextStream & output) const;

protected:
	void resetPatterns() const;

private:
	std::tr1::shared_ptr<Pattern> stimulusStart_;
	std::tr1::shared_ptr<Pattern> stimulusEnd_;
	QList<std::tr1::shared_ptr<Pattern> > stimulusPatterns_;
};

}

#endif
