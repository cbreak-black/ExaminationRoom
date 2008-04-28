/*
 *  object.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "object.h"
#include "container.h"
#include "scene.h"
#include "surfaces/abstracttexture.h"

#include <sstream>

namespace Examination
{
	using namespace Tool;

// Creation
Object::Object()
{
	origin_ = Point(0,0,0);
	init();
}

Object::Object(float x, float y, float z)
{
	origin_.x = x;
	origin_.y = y;
	origin_.z = z;
	init();
}

Object::Object(Point o)
{
	origin_ = o;
	init();
}

void Object::init()
{
	setScene(0);
	setParent(0);
	setColor(Color4(1, 1, 1, 1));
	setWireframe(false);
	setShown(true);
	setDrawPriority(0);
	setName("Object");
}

Object::~Object()
{
	unregisterUniqueName(name_);
}

Tool::Point Object::position() const
{
	return origin_;
}

void Object::setPosition(Tool::Point p)
{
	origin_ = p;
}

Scene * Object::scene() const
{
	return scene_;
}

void Object::setScene(Scene * s)
{
	scene_ = s;
}

Container * Object::parent() const
{
	return parent_;
}

void Object::setParent(Container * c)
{
	parent_ = c;
}

// Textures
void Object::setTexture(std::tr1::shared_ptr<AbstractTexture> t)
{
	tex_ = t;
}

std::tr1::shared_ptr<AbstractTexture> Object::texture() const
{
	return tex_;
}

Tool::Color4 Object::color() const
{
	return color_;
}

void Object::setColor(Tool::Color4 color)
{
	color_ = color;
}

bool Object::wireframe() const
{
	return wireframe_;
}

void Object::setWireframe(bool flag)
{
	wireframe_ = flag;
}

// Visibility
bool Object::shown() const
{
	return shown_;
}

void Object::setShown(bool shown)
{
	shown_ = shown;
}

bool Object::visible() const
{
	if (parent())
	{
		return shown() && parent()->visible();
	}
	else
	{
		return shown();
	}
}

// Drawing priority
int Object::drawPriority() const
{
	return drawPriority_;
}

void Object::setDrawPriority(int priority)
{
	drawPriority_ = priority;
	if (parent())
	{
		parent()->sortObjects();
	}
}

bool operator<(std::tr1::shared_ptr<Object> & a, std::tr1::shared_ptr<Object> & b)
{
	return a->drawPriority() < b->drawPriority();
}

// Name
const std::string & Object::name() const
{
	return name_;
}

bool Object::setName(const std::string & name)
{
	objectWillChange();
	unregisterUniqueName(name_);
	bool result = checkUniqueName(name);
	name_ = registerUniqueName(name);
	objectDidChange();
	return result;
}

// Unique Name Management (Static)
std::set<std::string> Object::uniqueNames_ = std::set<std::string>();

bool Object::checkUniqueName(const std::string & name)
{
	return uniqueNames_.find(name) == uniqueNames_.end();
}

std::string Object::registerUniqueName(const std::string & name)
{
	if (checkUniqueName(name))
	{
		// If the name is free, register it
		uniqueNames_.insert(name);
		return name;
	}
	else
	{
		// Search a free name
		int i = 1;
		std::string tempName = name;
		while (!checkUniqueName(tempName))
		{
			std::stringstream ss(name, std::ios_base::out | std::ios_base::app);
			ss << i;
			tempName = ss.str();
			i++;
		}
		// If the name is free, register it
		uniqueNames_.insert(tempName);
		return tempName;
	}
}

void Object::unregisterUniqueName(const std::string & name)
{
	uniqueNames_.erase(name);
}

// Serialisation
std::string Object::className() const
{
	return "Object";
}

std::string Object::toLua(std::ostream outStream) const
{
	return className();
}

std::string Object::toLuaCreate(std::ostream outStream) const
{
	return className();
}


// Callbacks
void Object::callCallbacks(const std::list<SignalCallbackType> & list, const Object * obj)
{
	std::list<SignalCallbackType>::const_iterator it = list.begin();
	std::list<SignalCallbackType>::const_iterator end = list.end();
	for (; it != end; it++)
	{
		(*it)(obj);
	}
}

void Object::addCallbackParameterChanged(const SignalCallbackType & cb)
{
	parameterChanged_.push_back(cb);
}

// Signals
void Object::objectWillChange() const
{
	if (scene())
		scene()->objectWillChange(this);
}

void Object::objectDidChange() const
{
	if (scene())
		scene()->objectDidChange(this);
	callCallbacks(parameterChanged_, this);
}


}
