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

#include <memory>

namespace Statistics
{
	class Log;

class LogModel : public QAbstractTableModel
{
public:
	LogModel();
	LogModel(std::tr1::shared_ptr<Log> log);

public:
	std::tr1::shared_ptr<Log> log() const;
	void setLog(std::tr1::shared_ptr<Log> log);

public: // Bare Bones
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
	std::tr1::shared_ptr<Log> log_;
};

}
#endif
