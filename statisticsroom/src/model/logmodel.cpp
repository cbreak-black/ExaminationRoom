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

namespace Statistics
{

int LogModel::rowCount(const QModelIndex &parent) const
{
	return 1;
}

int LogModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
	return QVariant();
}

}
