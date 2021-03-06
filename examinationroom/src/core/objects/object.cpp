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

#include "program.h"
#include "namemanager.h"

#include "luabridge.hpp"
#include "luahelper.h"

namespace Examination
{
	using namespace Tool;

// Creation
Object::Object()
{
	setPosition(Point(0,0,0));
	setColor(Color4(1, 1, 1, 1));
	setWireframe(false);
	setShown(true);
	setDrawPriority(0);
	setName("object");
}

Object::Object(const Object & o)
	: std::tr1::enable_shared_from_this<Object>(o)
{
	setPosition(o.position());
	setColor(o.color());
	setWireframe(o.wireframe());
	setShown(o.shown());
	setDrawPriority(o.drawPriority());
	setName(o.name());
	if (o.texture())
	{
		setTexture(o.texture()->clone());
	}
}

Object::~Object()
{
	if (scene() && scene()->program())
	{
		scene()->program()->nameManager()->unregisterName(name_);
	}
}

ObjectPtr Object::sharedPtr()
{
	try
	{
		return shared_from_this();
	}
	catch (std::tr1::bad_weak_ptr const & e)
	{
		// No pointer exists yet, return an empty one
		return ObjectPtr();
	}
}

ConstObjectPtr Object::sharedPtr() const
{
	try
	{
		return shared_from_this();
	}
	catch (std::tr1::bad_weak_ptr const & e)
	{
		// No pointer exists yet, return an empty one
		return ConstObjectPtr();
	}
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

std::tr1::shared_ptr<Scene const> Object::scene() const
{
	return scene_.lock();
}

std::tr1::shared_ptr<Scene> Object::scene()
{
	return scene_.lock();
}

void Object::setScene(std::tr1::shared_ptr<Scene> s)
{
	if (scene() && scene()->program())
	{
		scene()->program()->nameManager()->unregisterName(name());
	}
	scene_ = s;
	if (scene() && scene()->program())
	{
		// Re-set the name to ensure uniqueness
		std::tr1::shared_ptr<NameManager> nm = scene()->program()->nameManager();
		NamedItemPtr nip = nm->registerName(nm->sanitizeName(name()), sharedPtr());
		name_ = nip->name();
	}
}

std::tr1::shared_ptr<Container> Object::parent() const
{
	return parent_.lock();
}

void Object::setParent(std::tr1::shared_ptr<Container> c) const
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

bool operator<(ObjectPtr & a, ObjectPtr & b)
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
	bool result = true;
	if (scene() && scene()->program())
	{
		std::tr1::shared_ptr<NameManager> nm = scene()->program()->nameManager();
		nm->unregisterName(name_);
		std::string saneName = nm->sanitizeName(name);
		result = nm->checkUniqueName(saneName);
		NamedItemPtr nip = nm->registerName(saneName, sharedPtr());
		name_ = nip->name();
	}
	else
	{
		name_ = name;
	}
	objectDidChange();
	return result;
}

// Serialisation
const char * Object::className_ = "Object";

std::string Object::className() const
{
	return Object::className_;
}

std::string Object::toLua(std::ostream & outStream) const
{
	toLuaCreate(outStream);
	outStream << name() << ":" << "setName(\"" << name() << "\");\n";
	outStream << name() << ":" << "setPosition({"
		<< position().x << ", " << position().y << ", " << position().z << "});\n";
	outStream << name() << ":" << "setColor({"
		<< color().r << ", " << color().g << ", " << color().b << ", " << color().a << "});\n";
	outStream << name() << ":" << "setWireframe(" << (wireframe() ? "true" : "false") << ");\n";
	outStream << name() << ":" << "setShown(" << (shown() ? "true" : "false") << ");\n";
	outStream << name() << ":" << "setDrawPriority(" << drawPriority() << ");\n";
	if (texture())
	{
		// Use a local scope for the texture
		outStream << "do -- texture\nlocal ";
		std::string texName = texture()->toLua(outStream);
		outStream << name() << ":" << "setTexture(" << texName << ");\n";
		outStream << "end -- texture\n";
	}
	return name();
}

std::string Object::toLuaCreate(std::ostream & outStream) const
{
	outStream << std::endl << "-- Create " << name() << " of type " << className() << std::endl;
	outStream << name() << " = " << className() << "();\n";
	return name();
}

// LUA
void Object::registerLuaApi(luabridge::module * m)
{
	m->class_<Object>(Object::className_)
	.method("name", &Object::name)
	.method("setName", &Object::setName)
	.method("position", &Object::position)
	.method("setPosition", &Object::setPosition)
	.method("color", &Object::color)
	.method("setColor", &Object::setColor)
	.method("wireframe", &Object::wireframe)
	.method("setWireframe", &Object::setWireframe)
	.method("drawPriority", &Object::drawPriority)
	.method("setDrawPriority", &Object::setDrawPriority)
	.method("shown", &Object::shown)
	.method("setShown", &Object::setShown)
	.method("visible", &Object::visible)
	.method("texture", &Object::texture)
	.method("setTexture", &Object::setTexture);
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

Object & Object::operator=(const Object &)
{
	return *this;
}


}
