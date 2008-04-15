/*
 *  container.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "container.h"

#include "scene.h"
#include "objects/object.h"

using namespace std::tr1;

namespace Examination
{

Container::Container()
{
}

Container::~Container()
{
	clear();
}

bool Container::addObject(shared_ptr<Object> object)
{
	std::list< shared_ptr<Object> >::iterator it = objects_.begin();
	std::list< shared_ptr<Object> >::iterator end = objects_.end();
	// Check for duplicate insert. Return false if the object is already stored
	while (it != end)
	{
		if ((*it) == object)
		{
			return false;
		}
		it++;
	}
	layoutWillChange();
	// Search insert position.
	it = objects_.begin();
	while (it != end && (*it) < object)
	{
		it++;
	}
	// Insert either in front of the first object that is larger or equal
	// Or at the end, if it is the largest
	// Preserving sorted state
	objects_.insert(it, object);
	// Sorting is not needed since we never insert anything that violates sorting
	//objects_.sort();
	// Removing non unique objects is not needed since we test for uniqueness before inserting
	//objects_.unique();
	Container * p = object->parent();
	// If the object had a valid parent, and the parent was not us
	// We should not reach this place if the parent was us... due to the guard
	if (p && (p != this))
	{
		// remove it
		p->removeObject(object);
	}
	// Set new parent and scene
	object->setParent(getParent());
	object->setScene(getScene());
	layoutDidChange();
	return true;
}

void Container::removeObject(shared_ptr<Object> object)
{
	layoutWillChange();
	object->setParent(0);
	object->setScene(0);
	objects_.remove(object);
	layoutDidChange();
}

void Container::clear()
{
	layoutWillChange();
	setParentsAndScenes(0,0);
	objects_.clear();
	layoutDidChange();
}

void Container::sortObjects()
{
	layoutWillChange();
	objects_.sort();
	layoutDidChange();
}

Container * Container::getParent()
{
	return this;
}

Scene * Container::getScene()
{
	return scene();
}

void Container::setParentsAndScenes(Container * p, Scene * s)
{
	std::list<shared_ptr<Object> >::iterator i = objects_.begin();
	for (; i != objects_.end(); i++)
	{
		(*i)->setParent(p);
		(*i)->setScene(s);
	}
}

void Container::setScene(Scene * s)
{
	Object::setScene(s);
	setParentsAndScenes(parent(), scene());
}

void Container::setParent(Container * c)
{
	Object::setParent(c);
	setParentsAndScenes(parent(), scene());
}

// Drawing
void Container::draw(GLWidget * dest) const
{
	if (shown())
	{
		std::list<shared_ptr<Object> >::const_iterator i = objects_.begin();
		for (; i != objects_.end(); i++)
		{
			(*i)->draw(dest);
		}
	}
}

const Container::ObjectList & Container::objects() const
{
	return objects_;
}

void Container::layoutWillChange() const
{
	if (scene())
		scene()->layoutWillChange(this);
}

void Container::layoutDidChange() const
{
	if (scene())
		scene()->layoutDidChange(this);
}


}
