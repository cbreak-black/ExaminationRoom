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
	setName("Container");
	setEnabled(true);
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
	std::tr1::shared_ptr<Container> p = object->parent();
	// If the object had a valid parent, and the parent was not us
	// We should not reach this place if the parent was us... due to the guard
	if (p && (p.get() != this))
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
	object->setParent(std::tr1::shared_ptr<Container>());
	object->setScene(std::tr1::shared_ptr<Scene>());
	objects_.remove(object);
	layoutDidChange();
}

void Container::clear()
{
	layoutWillChange();
	setParentsAndScenes(std::tr1::shared_ptr<Container>(),std::tr1::shared_ptr<Scene>());
	objects_.clear();
	layoutDidChange();
}

void Container::sortObjects()
{
	layoutWillChange();
	objects_.sort();
	layoutDidChange();
}

std::tr1::shared_ptr<Container> Container::getParent()
{
	return std::tr1::dynamic_pointer_cast<Container,Object>(sharedPtr());
}

std::tr1::shared_ptr<Scene> Container::getScene()
{
	return scene();
}

void Container::setParentsAndScenes(std::tr1::shared_ptr<Container> p, std::tr1::shared_ptr<Scene> s)
{
	std::list<shared_ptr<Object> >::iterator i = objects_.begin();
	for (; i != objects_.end(); i++)
	{
		(*i)->setParent(p);
		(*i)->setScene(s);
	}
}

void Container::setScene(std::tr1::shared_ptr<Scene> s)
{
	Object::setScene(s);
	setParentsAndScenes(getParent(), getScene());
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

// Transformation status
bool Container::enabled() const
{
	return enabled_;
}

void Container::setEnabled(bool enabled)
{
	objectWillChange();
	enabled_ = enabled;
	objectDidChange();
}

// Serialisation
std::string Container::className() const
{
	return "Container";
}

std::string Container::toLua(std::ostream & outStream) const
{
	Object::toLua(outStream);
	outStream << name() << ":" << "setEnabled(" << (enabled() ? "true" : "false") << ");\n";
	// Store all sub objects
	outStream << "-- Start sub-objects of " << name() << std::endl;
	std::list<shared_ptr<Object> >::const_reverse_iterator i = objects_.rbegin();
	for (; i != objects_.rend(); i++)
	{
		std::string itemName = (*i)->toLua(outStream);
		outStream << name() << ":" << "addObject(" << itemName << ");\n";
	}
	outStream << "-- End sub-objects of " << name() << std::endl;

	return name();
}

// Object accessor
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
