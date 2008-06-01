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

class QTextStream;

namespace Statistics
{
	class LogLine;

class LogModel : public QAbstractTableModel
{
public:
	LogModel();
	LogModel(QTextStream & input);

public: // Bare Bones
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public: // Statistics
	void calculateStatistics(QTextStream & output);

public: // Factory Methods
	static std::tr1::shared_ptr<LogModel> logModelFromStream(QTextStream & input);

private:
	QList<std::tr1::shared_ptr<LogLine> > logTable_;
};

}
#endif
