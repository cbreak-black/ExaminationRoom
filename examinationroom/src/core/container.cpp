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
}

bool Container::addObject(shared_ptr<Object> object)
{
	if (objects_.insert(object).second)
	{
		return true;
	}
	return false;
}

void Container::removeObject(shared_ptr<Object> object)
{
	objects_.erase(object);
}

void Container::clear()
{
	objects_.clear();
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
