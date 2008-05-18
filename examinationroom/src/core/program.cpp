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

namespace Examination
{

Program::Program()
{
	scene_ = Object::Create<Scene>();
	luaProxy_ = std::tr1::shared_ptr<LuaProxy>(new LuaProxy(scene_));
	nameManager_ = std::tr1::shared_ptr<NameManager>(new NameManager());
}

Program::~Program()
{
}

std::tr1::shared_ptr<Program> Program::create()
{
	std::tr1::shared_ptr<Program> t(new Program());
	t->this_ = t;
	t->scene_->setProgram(t);
	t->registerComponents();
	return t;
}

std::tr1::shared_ptr<Program> Program::createFromLua(const std::string & path)
{
	std::tr1::shared_ptr<Program> t(new Program());
	t->this_ = t;
	t->scene_->setProgram(t);
	t->registerComponents();
	t->luaProxy_->runFile(path.c_str());
	return t;
}

template <typename O>
inline void registerObject(luabridge::module * m, std::tr1::shared_ptr<NameManager> nameManager)
{
	nameManager->registerName(NamedItemPtr(new NamedClass<O>(O::className_)));
	O::registerLuaApi(m);
}

void Program::registerComponents()
{
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

// Persistency
void Program::toLua(std::ostream & outStream) const
{
	scene_->toLua(outStream);
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

}
