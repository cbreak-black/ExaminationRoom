/*
 *  program.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 11.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "program.h"

#include "scene.h"
#include "luaproxy.h"
#include "namemanager.h"

#include "luabridge.hpp"

#include "objects/object.h"
#include "objects/container.h"
#include "objects/pixelplane.h"
#include "objects/rectangle.h"
#include "objects/parallelepiped.h"
#include "objects/mesh.h"
#include "objects/sphere.h"
#include "objects/text.h"
#include "objects/affinetransformation.h"
#include "objects/atmosphere.h"
#include "objects/atmosphere.h"
#include "objects/cameranode.h"
#include "objects/depthbuffer.h"
#include "objects/lightnode.h"

#include <iostream>

#include "logtool.h"

namespace Examination
{

Program::Program()
{
	scene_ = std::tr1::shared_ptr<Scene>(new Scene());
	luaProxy_ = std::tr1::shared_ptr<LuaProxy>(new LuaProxy());
	nameManager_ = std::tr1::shared_ptr<NameManager>(new NameManager());
	nameManager_->registerLuaKeywords();
	registerComponents();
}

Program::~Program()
{
}

std::tr1::shared_ptr<Program> Program::create()
{
	std::tr1::shared_ptr<Program> t(new Program());
	t->this_ = t;
	t->scene_->setProgram(t);
	t->luaProxy_->setProgram(t);
	return t;
}

std::tr1::shared_ptr<Program> Program::createFromLua(const std::string & path)
{
	std::tr1::shared_ptr<Program> t = create();
	t->loadLua(path, true);
	return t;
}

std::tr1::shared_ptr<Program> Program::revert() const
{
	return Program::createFromLua(this->rootFile());
}

void Program::loadLua(const std::string & path, bool root)
{
	LogTool::logMessage("Program::loadLua", path);
	if (path.empty()) return;
	if (root && rootFile_.empty())
	{
		rootFile_ = path;
	}
	else
	{
		luaFiles_.push_back(path);
	}
	if (callbackFileLoad_)
		callbackFileLoad_(path);
	luaProxy_->runFile(path.c_str());
}

const std::vector<std::string> & Program::loadedLuaFiles() const
{
	return luaFiles_;
}

const std::string & Program::rootFile() const
{
	return rootFile_;
}

bool Program::removeLua(const std::string & path)
{
	std::vector<std::string>::iterator it;
	for (it = luaFiles_.begin(); it != luaFiles_.end(); it++)
	{
		if (*it == path)
		{
			luaFiles_.erase(it);
			return true;
		}
	}
	return false;
}

bool Program::runString(const std::string & luaCode)
{
	return luaProxy_->runString(luaCode.c_str());
}

template <typename O>
inline void registerObject(luabridge::module * m, std::tr1::shared_ptr<NameManager> nameManager)
{
	nameManager->registerName(NamedItemPtr(new NamedClass<O>(O::className_)));
	O::registerLuaApi(m);
}

void Program::registerComponents()
{
	// Fill the LUA API and the NameManager with all classes
	luabridge::module m(luaProxy_->luaState());
	registerObject<Object>(&m, nameManager_);
	registerObject<Container>(&m, nameManager_);
	registerObject<Pixelplane>(&m, nameManager_);
	registerObject<Rectangle>(&m, nameManager_);
	registerObject<Parallelepiped>(&m, nameManager_);
	registerObject<Mesh>(&m, nameManager_);
	registerObject<Sphere>(&m, nameManager_);
	registerObject<Text>(&m, nameManager_);
	registerObject<AffineTransformation>(&m, nameManager_);
	registerObject<Atmosphere>(&m, nameManager_);
	registerObject<CameraNode>(&m, nameManager_);
	registerObject<DepthBuffer>(&m, nameManager_);
	registerObject<LightNode>(&m, nameManager_);

	// Fill the Factory with classes that are intended to be instanciated
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<Pixelplane>));
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<Rectangle>));
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<Parallelepiped>));
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<Mesh>));
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<Sphere>));
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<Text>));
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<AffineTransformation>));
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<Atmosphere>));
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<CameraNode>));
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<DepthBuffer>));
	factories_.push_back(ObjectFactoryPtr(new ObjectFactory<LightNode>));
}

// Events
void Program::onUpdate()
{
	luaProxy_->onUpdate();
}

void Program::onKeyDown(char k)
{
	luaProxy_->onKeyDown(k);
}

void Program::onKeyUp(char k)
{
	luaProxy_->onKeyUp(k);
}

void Program::writeLog(const std::string & msg) const
{
	if (callbackLog_)
		callbackLog_(msg);
}

void Program::writeError(const std::string & type, const std::string & msg) const
{
	if (callbackError_)
		callbackError_(type + ": " + msg);
}

void Program::setCallbackLog(const SignalStringType & callback)
{
	callbackLog_ = callback;
}

void Program::setCallbackError(const SignalStringType & callback)
{
	callbackError_ = callback;
}

void Program::setCallbackFileLoad(const SignalStringType & callback)
{
	callbackFileLoad_ = callback;
}

// Persistency
void Program::toLua(std::ostream & outStream) const
{
	scene_->toLua(outStream);
	if (luaFiles_.size() > 0)
	{
		// Only write file load statements if at least one file was loaded
		outStream << std::endl;
		outStream << "-- Loaded Files" << std::endl;
		std::vector<std::string>::const_iterator it = luaFiles_.begin();
		std::vector<std::string>::const_iterator end = luaFiles_.end();
		for (; it != end; it++)
		{
			outStream << "Scene:loadLua(\"" << *it << "\");" << std::endl;
		}
	}
}

// Accessors
std::tr1::shared_ptr<Scene> Program::scene() const
{
	return scene_;
}

std::tr1::shared_ptr<NameManager> Program::nameManager() const
{
	return nameManager_;
}

std::tr1::shared_ptr<LuaProxy> Program::luaProxy() const
{
	return luaProxy_;
}

std::tr1::shared_ptr<Program> Program::sharedPtr() const
{
	return this_.lock();
}

const std::vector<std::tr1::shared_ptr<ObjectFactoryBase> > & Program::factories() const
{
	return factories_;
}

}
