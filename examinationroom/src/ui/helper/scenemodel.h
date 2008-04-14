/*
 *  scenemodel.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 14.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SCENEMODEL_H
#define SCENEMODEL_H

#include <QAbstractItemModel>

#include <memory>

namespace Examination
{
	class Scene;

/**
This class wrapps a scene so that it can be used as data source for
a QTreeView.
*/
class SceneModel : public QAbstractItemModel
{
public:
	SceneModel(std::tr1::shared_ptr<Scene> scene);

public: // ItemModel API
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

private:
	std::tr1::shared_ptr<Scene> scene_;
};

}

#endif