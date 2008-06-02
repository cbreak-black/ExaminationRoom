/*
 *  logtransformermodel.h
 *  StatisticsRoom
 *
 *  Created by cbreak on 02.06.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef LOGTRANSFORMERMODEL_H
#define LOGTRANSFORMERMODEL_H

#include <QAbstractListModel>

#include <memory>

namespace Statistics
{
	class LogTransformer;

class LogTransformerModel : public QAbstractListModel
{
public:
	LogTransformerModel();
	LogTransformerModel(std::tr1::shared_ptr<LogTransformer> log);

public:
	std::tr1::shared_ptr<LogTransformer> transformer() const;
	void setLog(std::tr1::shared_ptr<LogTransformer> transformer);

public: // Bare Bones
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex & index) const;

private:
	std::tr1::shared_ptr<LogTransformer> transformer_;
};

}

#endif
