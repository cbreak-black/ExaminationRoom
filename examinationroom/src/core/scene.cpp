/*******************************************************************************

Examination Room is a flexible user evaluation tool

Copyright (C) 2008 Gerhard Roethlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************************/

#include "scene.h"

#include "object.h"

namespace Examination
{

Scene::Scene()
{
}

Scene::~Scene()
{
	std::set<Object*>::iterator i = objects_.begin();
	for (; i != objects_.end(); i++)
	{
		(*i)->release();
	}
	objects_.clear();
}

bool Scene::addObject(Object * object)
{
	if (objects_.insert(object).second)
	{
		object->retain();
		return true;
	}
	return false;
}

void Scene::removeObject(Object * object)
{
	objects_.erase(object);
	object->release();
}

void Scene::drawScene(GLWidget * dest)
{
	std::set<Object*>::iterator i = objects_.begin();
	for (; i != objects_.end(); i++)
	{
		(*i)->draw(dest);
	}
}


}
