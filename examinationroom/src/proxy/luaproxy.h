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

#include <sys/time.h>

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
	int clearScene(lua_State *L);
public: // Camera
	int setCameraPos(lua_State *L);
	int setCameraDir(lua_State *L);
	int setCameraFoV(lua_State *L);
	int setCameraSep(lua_State *L);

public: // Misc
	int setEventListener(lua_State *L);
	int log(lua_State *L);
	
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
	void onEvent(const char * event, double param);
	void onEvent(const char * event, char * param);

private:
	void error(const char * s1, const char * s2);
	int handleError(int err, const char * s);

private:
	lua_State * L_;

	Scene * scene_;

	struct timeval lastUpdate_;

public: // LUNA
	static const char className[];
	static const Luna<LuaProxy>::RegType Register[];
};

	
}
#endif
