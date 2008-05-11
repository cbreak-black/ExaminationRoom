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

namespace Examination
{

Program::Program()
{
	scene_ = std::tr1::shared_ptr<Scene>(new Scene());
	luaProxy_ = std::tr1::shared_ptr<LuaProxy>(new LuaProxy(scene_));
}

Program::~Program()
{
}

std::tr1::shared_ptr<Program> Program::create()
{
	std::tr1::shared_ptr<Program> t(new Program());
	t->this_ = t;
	return t;
}

std::tr1::shared_ptr<Program> Program::createFromLua(const std::string & path)
{
	std::tr1::shared_ptr<Program> t(new Program());
	t->this_ = t;
	t->luaProxy_->runFile(path.c_str());
	return t;
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

std::tr1::shared_ptr<NameManager> Program::names() const
{
	return names_;
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
