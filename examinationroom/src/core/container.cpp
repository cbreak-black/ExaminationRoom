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

void Container::setParentsAndScenes(Container * p, Scene * s)
{
	std::set<shared_ptr<Object> >::iterator i = objects_.begin();
	for (; i != objects_.end(); i++)
	{
		(*i)->setParent(p);
		(*i)->setScene(s);
	}
}

// Drawing
void Container::draw(GLWidget * dest) const
{
	std::set<shared_ptr<Object> >::iterator i = objects_.begin();
	for (; i != objects_.end(); i++)
	{
		(*i)->draw(dest);
	}
}

}
