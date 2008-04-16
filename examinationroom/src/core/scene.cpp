/*
 *  scene.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "scene.h"

#include "objects/object.h"
#include "camera.h"

using namespace std::tr1;

namespace Examination
{

Scene::Scene()
{
	camera_ = shared_ptr<Camera>(new Camera());
	setBackgroundColor(0,0,0,0);
	setScene(this);
	setName("Scene");
}

Scene::~Scene()
{
	objWillChangeCallbacks_.clear();
	objDidChangeCallbacks_.clear();
	layoutWillChangeCallbacks_.clear();
	layoutDidChangeCallbacks_.clear();
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
	setColor(Tool::Color4((float)r/255, (float)g/255, (float)b/255, (float)a/255));
}

void Scene::setBackgroundColor(Tool::Color4 c)
{
	setColor(c);
}

Tool::Color4 Scene::backgroundColor() const
{
	return color();
}

Container * Scene::getParent()
{
	return 0;
}

Scene * Scene::getScene()
{
	return this;
}

// Signals

void Scene::objectWillChange(const Object * obj) const
{
	callCallbacks(objWillChangeCallbacks_, obj);
}

void Scene::objectDidChange(const Object * obj) const
{
	callCallbacks(objDidChangeCallbacks_, obj);
}

void Scene::layoutWillChange(const Container * cont) const
{
	callCallbacks(layoutWillChangeCallbacks_, cont);
}

void Scene::layoutDidChange(const Container * cont) const
{
	callCallbacks(layoutDidChangeCallbacks_, cont);
}

void Scene::callCallbacks(const std::list<SignalCallbackType> & list, const Object * obj) const
{
	std::list<SignalCallbackType>::const_iterator it = list.begin();
	std::list<SignalCallbackType>::const_iterator end = list.end();
	for (; it != end; it++)
	{
		(*it)(obj);
	}
}

void Scene::addCallbackObjectWillChange(const SignalCallbackType & cb)
{
	objWillChangeCallbacks_.push_back(cb);
}

void Scene::addCallbackObjectDidChange(const SignalCallbackType & cb)
{
	objDidChangeCallbacks_.push_back(cb);
}

void Scene::addCallbackLayoutWillChange(const SignalCallbackType & cb)
{
	layoutWillChangeCallbacks_.push_back(cb);
}

void Scene::addCallbackLayoutDidChange(const SignalCallbackType & cb)
{
	layoutDidChangeCallbacks_.push_back(cb);
}

void Scene::removeCallbackObjectWillChange(const SignalCallbackType & cb)
{
	//objWillChangeCallbacks_.remove(cb);
}

void Scene::removeCallbackObjectDidChange(const SignalCallbackType & cb)
{
	//objDidChangeCallbacks_.remove(cb);
}

void Scene::removeCallbackLayoutWillChange(const SignalCallbackType & cb)
{
	//layoutWillChangeCallbacks_.remove(cb);
}

void Scene::removeCallbackLayoutDidChange(const SignalCallbackType & cb)
{
	//layoutDidChangeCallbacks_.remove(cb);
}


}
