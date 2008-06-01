/*
 *  LogModel.cpp
 *  StatisticsRoom
 *
 *  Created by CBreak on 29.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "logmodel.h"

#include "logline.h"

#include <QSize>
#include <QTextStream>

#include "pattern.h"

using namespace std::tr1;

namespace Statistics
{

LogModel::LogModel()
{
}

LogModel::LogModel(QTextStream & input)
{
	while (!input.atEnd())
	{
		QString s = input.readLine();
		shared_ptr<LogLine> l = LogLine::logLineFromString(s);
		if (l->isValid())
		{
			logTable_.append(l);
		}
	}
}

shared_ptr<LogModel> LogModel::logModelFromStream(QTextStream & input)
{
	return shared_ptr<LogModel>(new LogModel(input));
}

// Statistics
void LogModel::calculateStatistics(QTextStream & output)
{
	typedef shared_ptr<Pattern> PatternPtr;
	PatternPtr stimulusStart = PatternPtr(new Pattern("^New Target:.*$", "Start"));
	PatternPtr stimulusEnd = PatternPtr(new Pattern("^Input (?:Correct|Incorrect|Skipped): .*$", "End"));
	QList<PatternPtr> stimulusPatterns;
	stimulusPatterns.append(PatternPtr(new Pattern("^Input (Correct|Incorrect|Skipped): (.*)$", "Result;Answer")));
	stimulusPatterns.append(PatternPtr(new Pattern("^.*s=(-?\\d+\\.\\d+) deg$", "Separation")));
	stimulusPatterns.append(PatternPtr(new Pattern("^Target Properties:.*\\((-?\\d+\\.\\d+), (-?\\d+\\.\\d+), (-?\\d+\\.\\d+)\\).*$", "PosX;PosY;PosZ")));
	stimulusPatterns.append(PatternPtr(new Pattern("^New Cycle: .*$", "Cycle")));
	stimulusPatterns.append(PatternPtr(new Pattern("^New Block: .*$", "Block")));
	stimulusPatterns.append(PatternPtr(new Pattern("^New Block: (.*)$", "BlockLabel")));

	output << "Time (msec)";
	stimulusStart->printHeader(output);
	Q_FOREACH(PatternPtr pat, stimulusPatterns)
	{
		pat->printHeader(output);
	}
	output << "\n";

	QDateTime tStart;
	for (int i = 0; i < logTable_.size(); i++)
	{
		shared_ptr<LogLine> ll = logTable_.at(i);
		QString lm = ll->message();
		if (stimulusStart->match(lm))
		{
			tStart = ll->timestamp();
		}
		Q_FOREACH(PatternPtr pat, stimulusPatterns)
		{
			pat->match(lm);
		}
		if (stimulusEnd->match(lm))
		{
			output << tStart.time().msecsTo(ll->timestamp().time());
			stimulusStart->print(output);
			Q_FOREACH(PatternPtr pat, stimulusPatterns)
			{
				pat->print(output);
			}
			//stimulusEnd.print(output);
			output << "\n";
		}
	}
}

int LogModel::rowCount(const QModelIndex & /* parent */) const
{
	return logTable_.size();
}

int LogModel::columnCount(const QModelIndex & /* parent */) const
{
	return 3;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		int logLineIdx = index.row();
		switch (index.column())
		{
			case 0:
				return QVariant(logLineIdx);
			case 1:
				return QVariant(logTable_.at(logLineIdx)->timestamp().toString("hh:mm:ss.zzz"));
			case 2:
				return QVariant(logTable_.at(logLineIdx)->message());
		}
	}
	else if (role == Qt::SizeHintRole)
	{
		switch (index.column())
		{
			case 0:
				return QVariant(QSize(32, 18));
			case 1:
				return QVariant(QSize(128, 18));
			case 2:
				return QVariant(QSize(512, 18));
		}
	}
	return QVariant();
}

QVariant LogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			switch (section)
			{
				case 0:
					return QVariant("#");
				case 1:
					return QVariant(tr("Time Stamp"));
				case 2:
					return QVariant(tr("Message"));
			}
		}
		else if (role == Qt::SizeHintRole)
		{
			switch (section)
			{
				case 0:
					return QVariant(QSize(48, 18));
				case 1:
					return QVariant(QSize(128, 18));
				case 2:
					return QVariant(QSize(512, 18));
			}
		}
	}
	return QVariant();
}


}
