/*
 *  logtransformer.cpp
 *  StatisticsRoom
 *
 *  Created by cbreak on 02.06.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "logtransformer.h"

#include <QTextStream>
#include <QDateTime>

#include "pattern.h"
#include "log.h"
#include "logline.h"

namespace Statistics
{

typedef std::tr1::shared_ptr<Pattern> PatternPtr;

LogTransformer::LogTransformer()
{
	stimulusStart_ = PatternPtr(new Pattern("^New Target:.*$", "Start"));
	stimulusEnd_ = PatternPtr(new Pattern("^Input (?:Correct|Incorrect|Skipped): .*$", "End"));
	stimulusPatterns_.append(PatternPtr(new Pattern("^Input (Correct|Incorrect|Skipped): (.*)$", "Result;Answer")));
	stimulusPatterns_.append(PatternPtr(new Pattern("^.*s=(-?\\d+\\.\\d+) deg$", "Separation")));
	stimulusPatterns_.append(PatternPtr(new Pattern("^Target Properties:.*\\((-?\\d+\\.\\d+), (-?\\d+\\.\\d+), (-?\\d+\\.\\d+)\\).*$", "PosX;PosY;PosZ")));
	stimulusPatterns_.append(PatternPtr(new Pattern("^New Cycle: .*$", "Cycle")));
	stimulusPatterns_.append(PatternPtr(new Pattern("^New Block: .*$", "Block")));
	stimulusPatterns_.append(PatternPtr(new Pattern("^New Block: (.*)$", "BlockLabel")));
}

std::tr1::shared_ptr<Pattern> LogTransformer::stimulusStart()
{
	return stimulusStart_;
}

std::tr1::shared_ptr<Pattern> LogTransformer::stimulusEnd()
{
	return stimulusEnd_;
}

std::tr1::shared_ptr<Pattern> LogTransformer::stimulusData(int idx)
{
	if (idx >= 0 && idx < stimulusPatterns_.size())
		return stimulusPatterns_.at(idx);
	else
		return std::tr1::shared_ptr<Pattern>();
}

int LogTransformer::stimulusDataCount() const
{
	return stimulusPatterns_.size();
}

std::tr1::shared_ptr<Pattern> LogTransformer::pattern(int idx)
{
	if (idx == 0)
	{
		return stimulusStart_;
	}
	else if (idx == stimulusDataCount()+1)
	{
		return stimulusEnd_;
	}
	else
	{
		return stimulusData(idx-1);
	}
}

int LogTransformer::patternCount() const
{
	return stimulusDataCount() + 2;
}

void LogTransformer::resetPatterns() const
{
	stimulusStart_->reset();
	stimulusEnd_->reset();
	Q_FOREACH(PatternPtr pat, stimulusPatterns_)
	{
		pat->reset();
	}
}

// Statistics
void LogTransformer::transformLog(std::tr1::shared_ptr<Log> log, QTextStream & output) const
{
	if (!log) return;

	// Write headers
	output << "Time (msec)";
	stimulusStart_->printHeader(output);
	Q_FOREACH(PatternPtr pat, stimulusPatterns_)
	{
		pat->printHeader(output);
	}
	output << "\n";

	// Transform lines
	QDateTime tStart;
	for (int i = 0; i < log->size(); i++)
	{
		std::tr1::shared_ptr<LogLine> ll = log->at(i);
		QString lm = ll->message();
		if (stimulusStart_->match(lm))
		{
			tStart = ll->timestamp();
		}
		Q_FOREACH(PatternPtr pat, stimulusPatterns_)
		{
			pat->match(lm);
		}
		if (stimulusEnd_->match(lm))
		{
			// If the end pattern is matched, print the patterns
			output << tStart.time().msecsTo(ll->timestamp().time());
			stimulusStart_->print(output);
			Q_FOREACH(PatternPtr pat, stimulusPatterns_)
			{
				pat->print(output);
			}
			//stimulusEnd_.print(output);
			output << "\n";
		}
	}

	// Reset patterns for next transformation
	resetPatterns();
}


}
