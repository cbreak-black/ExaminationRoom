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

// Serialisation
std::string Scene::className() const
{
	return "Scene";
}

/**
\todo	Add Camera export
*/
std::string Scene::toLua(std::ostream & outStream) const
{
	// Localy scoped variable c points to camera
	std::string c = "cam";
	outStream << "do -- cam\nlocal " << c << " = " << name() << ":camera();\n";
	camera()->toLua(outStream, c);
	outStream << "end --cam\n";
	outStream << "Scene:" << "setBackgroundColor("
		<< (int)(color().r*255) << ", "
		<< (int)(color().g*255) << ", "
		<< (int)(color().b*255) << ", "
		<< (int)(color().a*255) << ");\n";
	// Store all sub objects
	outStream << "-- Start sub-objects of " << name() << std::endl;
	ObjectList::const_reverse_iterator i = objects().rbegin();
	for (; i != objects().rend(); i++)
	{
		std::string itemName = (*i)->toLua(outStream);
		outStream << "Scene:" << "addObject(" << itemName << ");\n";
	}
	outStream << "-- End sub-objects of Scene\n";
	return name();
}

std::string Scene::toLuaCreate(std::ostream & outStream) const
{
	outStream << "-- The Scene object exists from the Start\n";
	return name();
}

std::tr1::shared_ptr<Container> Scene::getParent()
{
	return std::tr1::dynamic_pointer_cast<Container,Object>(sharedPtr());
}

std::tr1::shared_ptr<Scene> Scene::getScene()
{
	return std::tr1::dynamic_pointer_cast<Scene,Object>(sharedPtr());
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

std::tr1::shared_ptr<Program> Scene::program() const
{
	return program_.lock();
}

void Scene::setProgram(std::tr1::shared_ptr<Program> program)
{
	program_ = program;
}


}
