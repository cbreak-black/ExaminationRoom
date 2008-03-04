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

std::tr1::shared_ptr<Camera> Scene::camera()
{
	return camera_;
}

}
