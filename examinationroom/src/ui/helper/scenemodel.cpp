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
#include <QMimeData>
#include <QStringList>

#include <sstream>

#include "objects/object.h"
#include "objects/container.h"
#include "scene.h"

namespace Examination
{

/**
A small helper class to perform internal drag&drops
*/
class ObjectMimeData : public QMimeData
{
public: // Creator
	ObjectMimeData() {};
public: // Accessors
	const QList<ObjectPtr> & objects() const
	{
		return objects_;
	}
	void addObject(ObjectPtr o)
	{
		objects_.append(o);
	}
private:
	QList<ObjectPtr> objects_;
};

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
		if ((!o->parent()) || std::tr1::dynamic_pointer_cast<Scene>(o->parent()))
		{
			// Has no real parent
			return QModelIndex();
		}
		else
		{
			std::tr1::shared_ptr<Container> p = o->parent();
			std::tr1::shared_ptr<Container> c = p->parent(); // Parent of the parent, to find parent's coordinates
			// c should always be valid, since every object has a parent besides the scene
			// and objects that have scene as parent were rejected above
			if (c)
			{
				Container::ObjectList::const_iterator it = c->objects().begin();
				Container::ObjectList::const_iterator end = c->objects().end();
				int i = 0;
				while (it != end && (*it) != p)
				{
					i++;
					it++;
				}
				return createIndex(i, 0, o->parent().get());
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
		return 3; // Could query the real object
	else
		return 3; // Scene itself
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
		else if (index.column() == 2)
		{
			if (role == Qt::CheckStateRole)
			{
				Object * o = static_cast<Object*>(index.internalPointer());
				Container * c = dynamic_cast<Container*>(o);
				if (c)
				{
					return QVariant(c->enabled() ? Qt::Checked : Qt::Unchecked);
				}
				else
				{
					return QVariant();
				}
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
		if (index.column() == 2 && role == Qt::CheckStateRole)
		{
			Object * o = static_cast<Object*>(index.internalPointer());
			Container * c = dynamic_cast<Container*>(o);
			if (c)
			{
				bool flag = value.toInt() == Qt::Checked;
				c->setEnabled(flag);
				dataChanged(index, index);
				return true;
			}
		}
	}
	return false;
}

Qt::ItemFlags SceneModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsDropEnabled;

	Object * o = static_cast<Object*>(index.internalPointer());
	Container * c = dynamic_cast<Container*>(o);
	if (index.column() == 0)
	{
		if (c)
		{
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable |
				Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
		}
		else
		{
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable |
				Qt::ItemIsDragEnabled;
		}
	}
	else if (index.column() == 1)
	{
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
	}
	else if (index.column() == 2)
	{
		if (c)
		{
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
		}
		else
		{
			return Qt::ItemIsSelectable;
		}
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
				case 2:
					return QVariant("Enabled");
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

//bool SceneModel::removeRows (int row, int count, const QModelIndex & parent)
//{
//	std::tr1::shared_ptr<Container> container;
//	if (!parent.isValid())
//	{
//		// Remove from scene
//		container = scene_;
//	}
//	else
//	{
//		std::tr1::shared_ptr<Object> o = (static_cast<Object*>(parent.internalPointer()))->sharedPtr();
//		container = std::tr1::dynamic_pointer_cast<Container>(o);
//		if (!container)
//		{
//			// Should not happen, all valid model indexes that are parents are containers
//			return false;
//		}
//	}
//	beginRemoveRows(parent, row, row+count-1);
//	bool r = container->removeObjectRange(row, count);
//	endRemoveRows();
//	return r;
//}

QStringList SceneModel::mimeTypes() const
{
	QStringList types;
	types.append("text/plain");
	return types;
}

QMimeData * SceneModel::mimeData(const QModelIndexList &indexes) const
{
	ObjectMimeData * omd = new ObjectMimeData;
	std::stringstream ss(std::ios_base::out);
	for (QModelIndexList::const_iterator it = indexes.begin();
		 it != indexes.end();
		 it++)
	{
		if (it->isValid() && it->column() == 0)
		{
			Object * o = static_cast<Object*>(it->internalPointer());
			o->toLua(ss);
			omd->addObject(o->sharedPtr());
		}
	}
	omd->setText(QString::fromStdString(ss.str()));
	return omd;
}

bool SceneModel::dropMimeData(const QMimeData *data, Qt::DropAction /* action */,
				  int row, int /* column */, const QModelIndex &parent)
{
//	const ObjectMimeData *myData = qobject_cast<const ObjectMimeData *>(data);
	const ObjectMimeData *myData = dynamic_cast<const ObjectMimeData *>(data);
	const QList<ObjectPtr> & objects = myData->objects();

	ContainerPtr dest;

	if (parent.isValid())
	{
		// Get an object pointer
		Object * p = static_cast<Object*>(parent.internalPointer());
		Container * c = dynamic_cast<Container*>(p);
		if (c) // Parent is a container
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
				dest = std::tr1::dynamic_pointer_cast<Container>(*it);
				if (!dest)
				{
					// Found object is not a container. Just use parent
					dest = std::tr1::dynamic_pointer_cast<Container>(c->sharedPtr());
				}
				// else // Found object is where we drop our items in
				//{
				//	dest = dest;
				//}
			}
			else
			{
				// Invalid index. Use parent as container
				dest = std::tr1::dynamic_pointer_cast<Container>(c->sharedPtr());
			}
		}
		else
		{
			// Parents should always be containers so this should not happen
			dest = scene_;
		}
	}
	else
	{
		// Invalid index means insert into scene
		dest = scene_;
	}
	for (QList<ObjectPtr>::const_iterator it = objects.begin();
		 it != objects.end();
		 it++)
	{
		dest->addObject(*it);
	}
	return true;
}

Qt::DropActions SceneModel::supportedDropActions() const
{
	return Qt::CopyAction;
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
	//reset();
}

}
