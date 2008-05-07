/*
 *  object.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "object.h"
#include "scene.h"
#include "surfaces/abstracttexture.h"

#include "parameter/parameterobject.h"

#include <sstream>

namespace Examination
{
	using namespace Tool;

// Creation
Object::Object()
{
	origin_ = Point(0,0,0);
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

std::tr1::shared_ptr<Object> Object::sharedPtr() const
{
	return this_.lock();
}

Tool::Point Object::position() const
{
	return origin_;
}

void Object::setPosition(Tool::Point p)
{
	objectWillChange();
	origin_ = p;
	objectDidChange();
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
	objectWillChange();
	tex_ = t;
	objectDidChange();
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
	objectWillChange();
	color_ = color;
	objectDidChange();
}

bool Object::wireframe() const
{
	return wireframe_;
}

void Object::setWireframe(bool flag)
{
	objectWillChange();
	wireframe_ = flag;
	objectDidChange();
}

// Visibility
bool Object::shown() const
{
	return shown_;
}

void Object::setShown(bool shown)
{
	objectWillChange();
	shown_ = shown;
	objectDidChange();
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
	objectWillChange();
	drawPriority_ = priority;
	if (parent())
	{
		parent()->sortObjects();
	}
	objectDidChange();
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
	std::string saneName = sanitizeName(name);
	bool result = checkUniqueName(saneName);
	name_ = registerUniqueName(saneName);
	objectDidChange();
	return result;
}

// Unique Name Management (Static)
std::set<std::string> Object::uniqueNames_ = std::set<std::string>();
const char saneCharacters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
const char saneCharactersFirst[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";

bool Object::checkUniqueName(const std::string & name)
{
	return uniqueNames_.find(name) == uniqueNames_.end();
}

std::string Object::sanitizeName(const std::string & name)
{
	std::string saneName = name;
	unsigned int pos = saneName.find_last_not_of(saneCharacters);
	while (pos != std::string::npos)
	{
		saneName[pos] = '_';
		pos = saneName.find_last_not_of(saneCharacters, pos);
	}
	pos = saneName.find_last_not_of(saneCharactersFirst, 0);
	if (pos != std::string::npos)
	{
		saneName[0] = '_';
	}
	return saneName;
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

std::string Object::toLua(std::ostream & outStream) const
{
	toLuaCreate(outStream);
	outStream << name() << ":" << "setName(\"" << name() << "\");\n";
	outStream << name() << ":" << "setPosition("
		<< position().x << ", " << position().y << ", " << position().z << ");\n";
	outStream << name() << ":" << "setColor("
		<< color().r << ", " << color().g << ", " << color().b << ", " << color().a << ");\n";
	outStream << name() << ":" << "setWireframe(" << (wireframe() ? "true" : "false") << ");\n";
	outStream << name() << ":" << "setShown(" << (shown() ? "true" : "false") << ");\n";
	outStream << name() << ":" << "setDrawPriority(" << drawPriority() << ");\n";
	if (texture())
	{
		std::string texName = texture()->toLua(outStream);
		outStream << name() << ":" << "setTexture(" << texName << ");\n";
	}
	return name();
}

std::string Object::toLuaCreate(std::ostream & outStream) const
{
	outStream << std::endl << "-- Create " << name() << " of type " << className() << std::endl;
	outStream << name() << " = Object(\"" << className() << "\");\n";
	return name();
}

// Parameter Dialog
std::tr1::shared_ptr<ParameterObject> Object::dialog()
{
	if (!dialog_)
	{
		dialog_ = createDialog();
	}
	return dialog_;
}

std::tr1::shared_ptr<ParameterObject> Object::createDialog()
{
	return std::tr1::shared_ptr<ParameterObject>(new ParameterObject(sharedPtr()));
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
