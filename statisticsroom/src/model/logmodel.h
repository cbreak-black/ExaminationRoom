/*
 *  logmodel.h
 *  StatisticsRoom
 *
 *  Created by CBreak on 29.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QAbstractTableModel>
#include <QList>

namespace Statistics
{
	class LogLine;

class LogModel : public QAbstractTableModel
{
public:
	LogModel();

public: // Bare Bones
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
	QList<LogLine> logTable_;
};

}
#endif
