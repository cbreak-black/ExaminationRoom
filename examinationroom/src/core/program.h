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
#include <functional>
#include <vector>
#include <fstream>

namespace Examination
{
	class Scene;
	class LuaProxy;
	class NameManager;
	class ObjectFactoryBase;

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
	with the constructor Program(path).
	Note that errors that happen during program load will not cached. To set up
	callbacks before loading the program, use create() and loadLua().
	 \return a shared_ptr to a new instance of Program
	*/
	static std::tr1::shared_ptr<Program> createFromLua(const std::string & path);

public: // LUA
	/**
	Loads and executes a lua file from the given path.
	 \param path	A string containing the path to a lua file
	*/
	void loadLua(const std::string & path);

private:
	/**
	This method is used by factories after program creation to register objects
	with the LUA engine, and both objects and keywords with the name manager.
	New Object subclasses have to be registered by adding an entry in this function.
	*/
	void registerComponents();

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

public: // Logging
	/**
	Writes an entry into the log.
	Log entries are time stamped exactly.
	The log file is created when the program is instanciated.
	 \param msg	The message to write to the log
	*/
	void writeLog(const std::string & msg) const;

	/**
	Writes an entry to standard output.
	Error messages are time stamped like log entries
	 \param type	The type of the error
	 \param msg		The error message
	*/
	void writeError(const std::string & type, const std::string & msg) const;

public: // Signals
	/**
	Listeners for log signals have to have this signature.
	*/
	typedef std::tr1::function<void (const std::string &)> SignalLogType;

	/**
	Sets the callback for log messages.
	Callbacks will be called every time an entry is written to the log.
	 \param callback	The callback that is to be registered, created with std::tr1::bind
	*/
	void setCallbackLog(const SignalLogType & callback);

	/**
	Sets the callback for errors.
	Callbacks will be called every time an error is written to standard output.
	 \param callback	The callback that is to be registered, created with std::tr1::bind
	*/
	void setCallbackError(const SignalLogType & callback);

public: // Persistency
	/**
	Writes lua code that can recreate the program to the given out stream
	*/
	virtual void toLua(std::ostream & outStream) const;

public: // Accessors
	std::tr1::shared_ptr<Scene> scene() const;
	std::tr1::shared_ptr<NameManager> nameManager() const;
	std::tr1::shared_ptr<LuaProxy> luaProxy() const;
	std::tr1::shared_ptr<Program> sharedPtr() const;
	const std::vector<std::tr1::shared_ptr<ObjectFactoryBase> > & factories() const;

private:
	std::tr1::shared_ptr<Scene> scene_;
	std::tr1::shared_ptr<NameManager> nameManager_;
	std::tr1::shared_ptr<LuaProxy> luaProxy_;
	std::vector<std::tr1::shared_ptr<ObjectFactoryBase> > factories_;
	mutable std::ofstream logOutStream_;

	SignalLogType callbackLog_;
	SignalLogType callbackError_;

private:
	std::tr1::weak_ptr<Program> this_;
};

}

#endif
