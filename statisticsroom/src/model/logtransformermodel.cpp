/*
 *  logtransformermodel.cpp
 *  StatisticsRoom
 *
 *  Created by cbreak on 02.06.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "logtransformermodel.h"

#include "logtransformer.h"
#include "pattern.h"

#include <QSize>

using namespace std::tr1;

namespace Statistics
{

LogTransformerModel::LogTransformerModel()
{
}

LogTransformerModel::LogTransformerModel(std::tr1::shared_ptr<LogTransformer> transformer)
{
	transformer_ = transformer;
}

std::tr1::shared_ptr<LogTransformer> LogTransformerModel::transformer() const
{
	return transformer_;
}

void LogTransformerModel::setLog(std::tr1::shared_ptr<LogTransformer> transformer)
{
	transformer_ = transformer;
	reset();
}

int LogTransformerModel::rowCount(const QModelIndex & /* parent */) const
{
	if (transformer_)
		return transformer_->patternCount();
	else
		return 0;
}

int LogTransformerModel::columnCount(const QModelIndex & /* parent */) const
{
	return 2;
}

QVariant LogTransformerModel::data(const QModelIndex &index, int role) const
{
	if (!transformer_) return QVariant();

	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		std::tr1::shared_ptr<Pattern> p = transformer_->pattern(index.row());
		if (!p) return QVariant(); // Should only get valid indexes
		switch (index.column())
		{
			case 0:
				return QVariant(p->pattern());
			case 1:
				return QVariant(p->fieldNames());
		}
	}
	return QVariant();
}

bool LogTransformerModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (role == Qt::EditRole && index.isValid() && transformer_)
	{
		std::tr1::shared_ptr<Pattern> p = transformer_->pattern(index.row());
		if (!p) return false; // Should only get valid indexes
		switch (index.column())
		{
			case 0:
				p->setPattern(value.toString());
				break;
			case 1:
				p->setFieldNames(value.toString());
				break;
			default:
				return false;
		}
		return true;
	}
	return false;
}

QVariant LogTransformerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole || role == Qt::EditRole)
		{
			switch (section)
			{
				case 0:
					return QVariant("Pattern");
				case 1:
					return QVariant(tr("Field Names"));
			}
		}
		else if (role == Qt::SizeHintRole)
		{
			switch (section)
			{
				case 0:
					return QVariant(QSize(400, 18));
				case 1:
					return QVariant(QSize(400, 18));
			}
		}
	}
	return QVariant();
}

Qt::ItemFlags LogTransformerModel::flags(const QModelIndex & index) const
{
	return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}


}
