/*
 *  LogModel.cpp
 *  StatisticsRoom
 *
 *  Created by CBreak on 29.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "logmodel.h"

#include "log.h"
#include "logline.h"

#include <QSize>

using namespace std::tr1;

namespace Statistics
{

LogModel::LogModel()
{
}

LogModel::LogModel(std::tr1::shared_ptr<Log> log)
{
	log_ = log;
}

std::tr1::shared_ptr<Log> LogModel::log() const
{
	return log_;
}

void LogModel::setLog(std::tr1::shared_ptr<Log> log)
{
	log_ = log;
	reset();
}

int LogModel::rowCount(const QModelIndex & /* parent */) const
{
	if (log_)
		return log_->size();
	else
		return 0;
}

int LogModel::columnCount(const QModelIndex & /* parent */) const
{
	return 3;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
	if (!log_) return QVariant();

	if (role == Qt::DisplayRole)
	{
		int logLineIdx = index.row();
		switch (index.column())
		{
			case 0:
				return QVariant(logLineIdx);
			case 1:
				return QVariant(log_->at(logLineIdx)->timestamp().toString("hh:mm:ss.zzz"));
			case 2:
				return QVariant(log_->at(logLineIdx)->message());
		}
	}
	else if (role == Qt::SizeHintRole)
	{
		switch (index.column())
		{
			case 0:
				return QVariant(QSize(32, 14));
			case 1:
				return QVariant(QSize(128, 14));
			case 2:
				return QVariant(QSize(512, 14));
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
