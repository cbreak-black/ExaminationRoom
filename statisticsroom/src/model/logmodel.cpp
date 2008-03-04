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
#include "tools/vec.h"

#include <QSize>
#include <QRegExp>

using namespace std::tr1;
using namespace Tool;

namespace Statistics
{

LogModel::LogModel()
{
}

LogModel::LogModel(QTextStream * input)
{
	while (!input->atEnd())
	{
		QString s = input->readLine();
		shared_ptr<LogLine> l = LogLine::logLineFromString(s);
		if (l->isValid())
		{
			logTable_.append(l);
		}
	}
}

shared_ptr<LogModel> LogModel::logModelFromStream(QTextStream * input)
{
	return shared_ptr<LogModel>(new LogModel(input));
}

// Statistics
void LogModel::calculateStatistics(QTextStream * output)
{
	QRegExp stimulusStart = QRegExp("^New Q:.*$");
	QRegExp stimulusEnd = QRegExp("^Input (?:Correct|Incorrect|Skipped): .*$");
	QRegExp stimulusCorrect = QRegExp("^Input (Correct|Incorrect|Skipped): (.*)$");
	QRegExp stimulusSeparation = QRegExp("^.*s=(-?\\d+\\.\\d+) deg$");
	QRegExp stimulusNewCycle = QRegExp("^New Test Cycle$");
	QRegExp stimulusPosition = QRegExp("^New Q:.*\\((-?\\d+\\.\\d+), (-?\\d+\\.\\d+), (-?\\d+\\.\\d+)\\).*$");

	QList<int> listTrials;
	QList<QString> listCorrect;
	QList<float> listSeparation;
	QList<float> listSeparationChange;
	QList<int> listCycleNumber;
	QList<Point> listPositions;

	QDateTime tStart;
	QString sCorrect;
	float separation = 0;
	float separationChange = 0;
	int cycleNumber = 0;
	Point position;
	for (int i = 0; i < logTable_.size(); i++)
	{
		shared_ptr<LogLine> ll = logTable_.at(i);
		QString lm = ll->message();
		if (stimulusStart.exactMatch(lm))
		{
			tStart = ll->timestamp();
		}
		if (stimulusCorrect.exactMatch(lm))
		{
			sCorrect = stimulusCorrect.cap(1) + "\t" + stimulusCorrect.cap(2);
		}
		if (stimulusSeparation.exactMatch(lm))
		{
			float s = stimulusSeparation.cap(1).toFloat();
			separationChange = s - separation;
			separation = s;
		}
		if (stimulusNewCycle.exactMatch(lm))
		{
			cycleNumber++;
		}
		if (stimulusPosition.exactMatch(lm))
		{
			position = Point(stimulusPosition.cap(1).toFloat(),
							 stimulusPosition.cap(2).toFloat(),
							 stimulusPosition.cap(3).toFloat());
		}
		if (stimulusEnd.exactMatch(lm))
		{
			listTrials.append(tStart.time().msecsTo(ll->timestamp().time()));
			listCorrect.append(sCorrect);
			listSeparation.append(separation);
			listSeparationChange.append(separationChange);
			listCycleNumber.append(cycleNumber);
			listPositions.append(position);
			sCorrect = "";
			tStart = QDateTime();
		}
	}

	(*output) << "#\t"
		<< "Time (msec)\t"
		<< "Result\tConditions\t"
		<< "Separation\t"
		<< "Separation Change\t"
		<< "Position (x\ty\tz)\t"
		<< "Cycle Number\n";
	for (int i = 0; i < listTrials.size(); i++)
	{
		(*output) << i << "\t"
			<< listTrials.at(i) << "\t"
			<< listCorrect.at(i) << "\t"
			<< listSeparation.at(i) << "\t"
			<< listSeparationChange.at(i) << "\t"
			<< listPositions.at(i).x << "\t" << listPositions.at(i).y << "\t" << listPositions.at(i).z << "\t"
			<< listCycleNumber.at(i) << "\n";
	}
}

int LogModel::rowCount(const QModelIndex &parent) const
{
	return logTable_.size();
}

int LogModel::columnCount(const QModelIndex &parent) const
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
