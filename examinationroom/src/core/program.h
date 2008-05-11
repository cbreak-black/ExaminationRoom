/*
 *  program.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 11.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROGRAM_H
#define PROGRAM_H

#include <memory>

namespace Examination
{
	class Scene;
	class LuaProxy;
	class NameManager;

class Program
{
protected:
	/**
	Creates a new empty program.
	 /warning Don't call this directly, use the factory methods
	*/
	Program();

public:
	/**
	Virtual destructor which does nothing.
	All referenced memory is managed by shared_ptr, which releases it once all
	references go out of scope.
	*/
	virtual ~Program();

public: // Factory methods
	/**
	Factory metod.
	Returns a shared_ptr to a newly allocated instance of Program.
	 \return a shared_ptr to a new instance of Program
	*/
	static std::tr1::shared_ptr<Program> create();

	/**
	Factory method.
	Returns a shared_ptr to a newly allocated instance of Program. It is initialized
	with the constructor Program(path)
	 \return a shared_ptr to a new instance of Program
	*/
	static std::tr1::shared_ptr<Program> createFromLua(const std::string & path);

public: // Events
	/**
	Call to signal the lua world that the scene has to be updated.
	*/
	virtual void onUpdate();

	/**
	Call to signal a key press.
	 \param k	The pressed key.
	*/
	virtual void onKeyDown(char k);

	/**
	Call to signal a key release.
	 \param k	The released key.
	*/
	virtual void onKeyUp(char k);

public: // Persistency
	/**
	Writes lua code that can recreate the program to the given out stream
	*/
	virtual void toLua(std::ostream & outStream) const;

public: // Accessors
	std::tr1::shared_ptr<Scene> scene() const;
	std::tr1::shared_ptr<NameManager> names() const;
	std::tr1::shared_ptr<LuaProxy> luaProxy() const;
	std::tr1::shared_ptr<Program> sharedPtr() const;

private:
	std::tr1::shared_ptr<Scene> scene_;
	std::tr1::shared_ptr<NameManager> names_;
	std::tr1::shared_ptr<LuaProxy> luaProxy_;

private:
	std::tr1::weak_ptr<Program> this_;
};

}

#endif