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

void Scene::setCamera(std::tr1::shared_ptr<Camera> camera)
{
	camera_ = camera;
}

std::tr1::shared_ptr<Camera> Scene::camera() const
{
	return camera_;
}

void Scene::setBackgroundColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	backgroundColor_.r = r;
	backgroundColor_.g = g;
	backgroundColor_.b = b;
	backgroundColor_.a = a;
}

void Scene::setBackgroundColor(Tool::Vec4f c)
{
	backgroundColor_ = c;
}

Tool::Vec4f Scene::backgroundColor() const
{
	return backgroundColor_;
}

}
