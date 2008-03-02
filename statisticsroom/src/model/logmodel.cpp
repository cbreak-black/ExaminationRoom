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


int LogModel::rowCount(const QModelIndex &parent) const
{
	return logTable_.size();
}

int LogModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		int logLineIdx = index.row();
		switch (index.column())
		{
			case 0:
				return QVariant(logTable_.at(logLineIdx)->timestamp());
				break;
			case 1:
				return QVariant(logTable_.at(logLineIdx)->message());
				break;
		}
	}
	else if (role == Qt::SizeHintRole)
	{
		switch (index.column())
		{
			case 0:
				return QVariant(QSize(128, 18));
				break;
			case 1:
				return QVariant(QSize(512, 18));
				break;
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
					return QVariant(tr("Time Stamp"));
					break;
				case 1:
					return QVariant(tr("Message"));
					break;
			}
		}
		else if (role == Qt::SizeHintRole)
		{
			switch (section)
			{
				case 0:
					return QVariant(QSize(128, 18));
					break;
				case 1:
					return QVariant(QSize(512, 18));
					break;
			}
		}
	}
	return QVariant();
}


}
