/*
 *  container.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "container.h"

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
	if (objects_.insert(object).second)
	{
		object->setParent(getParent());
		object->setScene(getScene());
		return true;
	}
	return false;
}

void Container::removeObject(shared_ptr<Object> object)
{
	object->setParent(0);
	object->setScene(0);
	objects_.erase(object);
}

void Container::clear()
{
	setParentsAndScenes(0,0);
	objects_.clear();
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
	std::set<shared_ptr<Object> >::iterator i = objects_.begin();
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
		std::set<shared_ptr<Object> >::iterator i = objects_.begin();
		for (; i != objects_.end(); i++)
		{
			(*i)->draw(dest);
		}
	}
}

}
