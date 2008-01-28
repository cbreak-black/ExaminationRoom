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

using namespace std::tr1;

namespace Examination
{

Scene::Scene()
{
}

Scene::~Scene()
{
	clear();
}

bool Scene::addObject(shared_ptr<Object> object)
{
	if (objects_.insert(object).second)
	{
		return true;
	}
	return false;
}

void Scene::removeObject(shared_ptr<Object> object)
{
	objects_.erase(object);
}

void Scene::clear()
{
	objects_.clear();
}

void Scene::drawScene(GLWidget * dest)
{
	std::set<shared_ptr<Object> >::iterator i = objects_.begin();
	for (; i != objects_.end(); i++)
	{
		(*i)->draw(dest);
	}
}


}
