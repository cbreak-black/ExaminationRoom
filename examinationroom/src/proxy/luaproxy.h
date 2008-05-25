/*
 *  luaproxy.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 26.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef LUAPROXY_H
#define LUAPROXY_H

#include "lua.hpp"

#include "vec.h"

#include <QTime>
#include <fstream>
#include <memory>

namespace Examination
{
	class Scene;
	class Object;
	class Container;
	class Camera;

/**
\defgroup LUA	LUA Bindings
LUA is the embedded scripting language that drives this tool. While all the functionality
of the tool is coded in C++/C, the scenes themselfs are interpreted code. Several classes
bind the two systems together.
*/

/**
This proxy class offers an interface between lua and C++. It also represents
The Scene in LUA.
 \ingroup LUA
*/
class LuaProxy
{
public:
	/**
	Creates the proxy and initializes lua.
	*/
	LuaProxy(std::tr1::shared_ptr<Scene> scene);
	
	/**
	Cleans up the lua environment.
	*/
	~LuaProxy();

public: // From C++
	/**
	Loads a lua file from disc, and executes it's contents.
	 \param path	Path to a lua file
	 \return 0 on success, error code on failure.
	*/
	int runFile(const char * path);
	
	/**
	Loads a lua string from memory, and executes it's contents.
	 \param code	Pointer to LUA script code
	 \return 0 on success, error code on failure.
	 */
	int runString(const char * code);

	/**
	Returns the associated lua state.
	*/
	lua_State * luaState() const;

public: // Scene
	void addObject(std::tr1::shared_ptr<Object> object);
	void removeObject(std::tr1::shared_ptr<Object> object);
	void clearScene();
	std::tr1::shared_ptr<Container> split();
	void merge(std::tr1::shared_ptr<Container> c);
	std::tr1::shared_ptr<Container> clone();
	void setBackgroundColor(char red, char green, char blue, char alpha);
public: // Camera
	void setCameraPos(float x, float y, float z);
	void setCameraDir(float x, float y, float z);
	void setCameraUp(float x, float y, float z);
	void setCameraFoV(float fov);
	void setCameraSep(float sep);
	void setCameraParalaxPlane(float pp);
	Tool::Point getCameraPos();
	Tool::Vec3f getCameraDir();
	Tool::Vec3f getCameraUp();
	float getCameraFoV();
	float getCameraSep();
	float getCameraParalaxPlane();
	// Access camera directly
	std::tr1::shared_ptr<Camera> camera() const;
	void setCamera(std::tr1::shared_ptr<Camera> camera);

public: // Statistical helpers
	float getSeparationAtPoint(Tool::Point p);
	float getUnitScreenSize(Tool::Point p);
	int getViewport(lua_State *L);

public: // Low Level Wrapper
	void beep();
	void exit();

public: // Misc
	int setEventListener(lua_State *L);
	void log(const char * msg);
	void debugLog(const char * msg);

public:
	/**
	Call to signal the lua world that the scene has to be updated.
	*/
	void onUpdate();
	
	/**
	Call to signal a key press.
	 \param k	The pressed key.
	*/
	void onKeyDown(char k);
	
	/**
	Call to signal a key release.
	 \param k	The released key.
	*/
	void onKeyUp(char k);

protected:
	/**
	Called by the destructor to signal termination of the scene.
	*/
	void onQuit();

protected:
	/**
	Called by high level event triggers to invoke an event with double parameter.
	 \param event	A string identifier of an event.
	 \param param	A double that is passed to the event handler
	*/
	void onEvent(const char * event, double param);

	/**
	Called by high level event triggers to invoke an event with string parameter.
	 \param event	A string identifier of an event.
	 \param param	A c string that is passed to the event handler
	*/
	void onEvent(const char * event, char * param);

private:
	void error(const char * s1, const char * s2);
	int handleError(int err, const char * s);

private:
	lua_State * L_;

	std::tr1::shared_ptr<Scene> scene_;

	QTime lastUpdate_;

	std::ofstream logOutStream_;

};

	
}
#endif
