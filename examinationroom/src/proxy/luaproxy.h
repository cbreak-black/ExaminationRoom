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

#include "luna.h"

#include <QTime>
#include <fstream>

namespace Examination
{
	class Scene;

/**
This proxy class offers an interface between lua and C++. It also represents
The Scene in LUA.
*/
class LuaProxy
{
public:
	/**
	Creates the proxy and initializes lua.
	*/
	LuaProxy(Scene * scene);
	
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

public: // Scene
	int addObject(lua_State *L);
	int removeObject(lua_State *L);
	int clearScene(lua_State *L);
	int setBackgroundColor(lua_State *L);
public: // Camera
	int setCameraPos(lua_State *L);
	int setCameraDir(lua_State *L);
	int setCameraFoV(lua_State *L);
	int setCameraSep(lua_State *L);
	int setCameraParalaxPlane(lua_State *L);
	int getCameraPos(lua_State *L);
	int getCameraDir(lua_State *L);
	int getCameraFoV(lua_State *L);
	int getCameraSep(lua_State *L);
	int getCameraParalaxPlane(lua_State *L);

public: // Statistical helpers
	int getSeparationAtPoint(lua_State *L);
	int getUnitScreenSize(lua_State *L);
	int getViewport(lua_State *L);

public: // Misc
	int setEventListener(lua_State *L);
	int log(lua_State *L);
	int debugLog(lua_State *L);
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
	void onEvent(const char * event, double param);
	void onEvent(const char * event, char * param);

private:
	void error(const char * s1, const char * s2);
	int handleError(int err, const char * s);

private:
	lua_State * L_;

	Scene * scene_;

	QTime lastUpdate_;

	std::ofstream logOutStream_;

public: // LUNA
	static const char className[];
	static const Luna<LuaProxy>::RegType Register[];
};

	
}
#endif
