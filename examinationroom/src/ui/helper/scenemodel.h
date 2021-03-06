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
	class Object;

/**
This class wrapps a scene so that it can be used as data source for a QTreeView.
If the visual representation has to be changed, it has to be done here.
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
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
//	virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

public: // D&D
	virtual QStringList mimeTypes() const;
	virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column, const QModelIndex &parent);
    virtual Qt::DropActions supportedDropActions() const;

public: // Editing
	virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

public: // Bindable callbacks for events
	void objectWillChange(const Object *);
	void objectDidChange(const Object *);
	void layoutWillChange(const Object *);
	void layoutDidChange(const Object *);

private:
	std::tr1::shared_ptr<Scene> scene_;
};

}

#endif