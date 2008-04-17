/*
 *  scenemodel.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 14.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "scenemodel.h"

#include <QSize>

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
	Container * c;
	if (parent.isValid())
	{
		// Get an object pointer
		Object * o = static_cast<Object*>(parent.internalPointer());
		// Check if this is a container object
		c = dynamic_cast<Container*>(o);
	}
	else
	{
		// Use scene as parent if an invalid index is specified as parent
		c = scene_.get();
	}
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
			Container * p = o->parent();
			Container * c = p->parent(); // Parent of the parent, to find parent's coordinates
			// c should always be valid, since every object has a parent besides the scene
			// and objects that have scene as parent were rejected above
			if (c)
			{
				Container::ObjectList::const_iterator it = c->objects().begin();
				Container::ObjectList::const_iterator end = c->objects().end();
				int i = 0;
				while (it != end && (*it).get() != p)
				{
					i++;
					it++;
				}
				return createIndex(i, 0, o->parent());
			}
			else
			{
				return QModelIndex();
			}
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
	{
		if (index.column() == 0)
		{
			if (role == Qt::DisplayRole)
			{
				Object * o = static_cast<Object*>(index.internalPointer());
				return QVariant(QString::fromStdString(o->name()));
			}
			else if (role == Qt::SizeHintRole)
			{
				return QVariant(QSize(192,18));
			}
			else
			{
				// Not used role
				return QVariant();
			}
		}
		else if (index.column() == 1)
		{
			if (role == Qt::CheckStateRole)
			{
				Object * o = static_cast<Object*>(index.internalPointer());
				return QVariant(o->shown() ? Qt::Checked : Qt::Unchecked);
			}
			else if (role == Qt::SizeHintRole)
			{
				return QVariant(QSize(28,18));
			}
			else
			{
				// Not used role
				return QVariant();
			}
		}
		else
		{
			// Not used column
			return QVariant();
		}
	}
	else
	{
		// Not valid
		return QVariant();
	}
}

bool SceneModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (index.isValid() && value.isValid())
	{
		if (index.column() == 1 && role == Qt::CheckStateRole)
		{
			Object * o = static_cast<Object*>(index.internalPointer());
			bool flag = value.toInt() == Qt::Checked;
			o->setShown(flag);
			dataChanged(index, index);
			return true;
		}
	}
	return false;
}

Qt::ItemFlags SceneModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	if (index.column() == 0)
	{
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}
	else if (index.column() == 1)
	{
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
	}
	else
	{
		return 0;
	}
}

QVariant SceneModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			switch (section)
			{
				case 0:
					return QVariant("Name");
					break;
				case 1:
					return QVariant("Shown");
					break;
				default:
					return QVariant();
			}
		}
		else
		{
			return QVariant();
		}
	}
	else
	{
		return QVariant();
	}
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