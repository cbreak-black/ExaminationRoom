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
#include <QRegExp>

using namespace std::tr1;

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
	QRegExp stimulusEnd = QRegExp("^Input (?:Correct|Incorrect): .*$");
	QRegExp stimulusCorrect = QRegExp("^Input (Correct|Incorrect): (.*)$");
	QRegExp stimulusSeperation = QRegExp("^.*s=(-?\\d+\\.\\d+) deg$");
	QRegExp stimulusNewCycle = QRegExp("^New Test Cycle$");

	QList<int> listTrials;
	QList<QString> listCorrect;
	QList<float> listSeperation;
	QList<float> listSeperationChange;
	QList<int> listCycleNumber;

	QDateTime tStart;
	QString sCorrect;
	float seperation = 0;
	float seperationChange = 0;
	int cycleNumber = 0;
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
		if (stimulusSeperation.exactMatch(lm))
		{
			float s = stimulusSeperation.cap(1).toFloat();
			seperationChange = s - seperation;
			seperation = s;
		}
		if (stimulusNewCycle.exactMatch(lm))
		{
			cycleNumber++;
		}
		if (stimulusEnd.exactMatch(lm))
		{
			listTrials.append(tStart.time().msecsTo(ll->timestamp().time()));
			listCorrect.append(sCorrect);
			listSeperation.append(seperation);
			listSeperationChange.append(seperationChange);
			listCycleNumber.append(cycleNumber);
			sCorrect = "";
			tStart = QDateTime();
		}
	}

	(*output) << "Time (msec)\t" << "Result\tConditions\t" << "Seperation\t" << "Seperation Change\t" << "Cycle Number\n";
	for (int i = 0; i < listTrials.size(); i++)
	{
		(*output) << listTrials.at(i) << "\t"
			<< listCorrect.at(i) << "\t"
			<< listSeperation.at(i) << "\t"
			<< listSeperationChange.at(i) << "\t"
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
					return QVariant(QSize(32, 18));
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
