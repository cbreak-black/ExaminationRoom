/*
 *  scene.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "scene.h"

#include "object.h"
#include "camera.h"

using namespace std::tr1;

namespace Examination
{

Scene::Scene()
{
	camera_ = shared_ptr<Camera>(new Camera());
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

void Scene::setCamera(std::tr1::shared_ptr<Camera> camera)
{
	camera_ = camera;
}

std::tr1::shared_ptr<Camera> Scene::camera()
{
	return camera_;
}

}
