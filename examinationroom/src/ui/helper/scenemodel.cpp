/*
 *  scenemodel.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 14.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "scenemodel.h"

#include "object.h"
#include "container.h"
#include "scene.h"

namespace Examination
{

SceneModel::SceneModel(std::tr1::shared_ptr<Scene> scene)
{
	scene_ = scene;
	scene_->addCallbackLayoutWillChange(std::tr1::bind(&SceneModel::layoutWillChange, this, _1));
	scene_->addCallbackLayoutDidChange(std::tr1::bind(&SceneModel::layoutDidChange, this, _1));
}

// Item Model API
QModelIndex SceneModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid())
	{
		// Get an object pointer
		Object * o = static_cast<Object*>(parent.internalPointer());
		// Check if this is a container object
		Container * c = dynamic_cast<Container*>(o);
		if (c)
		{
			// Get the specified sub object
			Container::ObjectList ol = c->objects();
			if (row >= 0 && row < (int)ol.size())
			{
				Container::ObjectList::const_iterator it = ol.begin();
				int itCounter = 0;
				while (itCounter < row)
				{
					itCounter++;
					it++;
				}
				return createIndex(row, column, (*it).get());
			}
			else
			{
				// Out of child list index
				return QModelIndex();
			}
		}
		else
		{
			// Not a container
			return QModelIndex();
		}
	}
	else
	{
		return createIndex(row, column, scene_.get());
	}
}

QModelIndex SceneModel::parent(const QModelIndex &child) const
{
	if (child.isValid())
	{
		Object * o = static_cast<Object*>(child.internalPointer());
		// If it has no parent, or a scene as parent
		if ((!o->parent()) || dynamic_cast<Scene*>(o->parent()))
		{
			// Has no real parent
			return QModelIndex();
		}
		else
		{
			// TODO: Find real row and column
			return createIndex(0, 0, o->parent());
		}
	}
	else
	{
		return QModelIndex();
	}
}

int SceneModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
	{
		return scene_->objects().size();
	}
	else
	{
		Object * o = static_cast<Object*>(parent.internalPointer());
		Container * c = dynamic_cast<Container*>(o);
		if (c)
		{
			return c->objects().size();
		}
		else
		{
			return 0;
		}
	}
}

int SceneModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 2; // Could query the real object
	else
		return 2; // Scene itself
}

QVariant SceneModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid())
		return QVariant();
	else
		return QVariant();
}

Qt::ItemFlags SceneModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

// Callbacks
void SceneModel::objectWillChange(const Object *)
{
}

void SceneModel::objectDidChange(const Object *)
{
}

void SceneModel::layoutWillChange(const Object *)
{
	layoutAboutToBeChanged();
}

void SceneModel::layoutDidChange(const Object *)
{
	layoutChanged();
}

}