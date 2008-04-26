#ifndef LUNA_H
#define LUNA_H 1

/*****************************************************************************
 *     .:: Luna ::.                                                          *
 *                                                                *   , *    *
 *  C++ library for binding classes into Lua.     By nornagon.       ((   *  *
 *                                                               *    `      *
 *  Example:                                                                 *
 *****************************************************************************

    class Foo {
      public:
        Foo(lua_State *L) {
          printf("in constructor\n");
        }

        int foo(lua_State *L) {
          printf("in foo\n");
        }

        ~Foo() {
          printf("in destructor\n");
        }

        static const char className[];
        static const Luna<Foo>::RegType Register[];
    };

    const char Foo::className[] = "Foo";
    const Luna<Foo>::RegType Foo::Register[] = {
      { "foo", &Foo::foo },
      { 0 }
    };

 *****************************************************************************
 * Then:                                                                     *
 *****************************************************************************

    Luna<Foo>::Register(L);

 *****************************************************************************
 * From Lua:                                                                 *
 *****************************************************************************

    local foo = Foo()
    foo:foo()

 *****************************************************************************
 * Clean up:                                                                 *
 *****************************************************************************

    lua_close(L);

 *****************************************************************************
 * Output:                                                                   *
 *****************************************************************************

    in constructor
    in foo
    in destructor

 *****************************************************************************
 * This program is free software. It comes without any warranty, to          *
 * the extent permitted by applicable law. You can redistribute it           *
 * and/or modify it under the terms of the Do What The Fuck You Want         *
 * To Public License, Version 2, as published by Sam Hocevar. See            *
 * http://sam.zoy.org/wtfpl/COPYING for more details.                        *
 ****************************************************************************/
 

// convenience macros
#define luna_register(L, klass) (Luna<klass>::Register((L)))
#define luna_registermetatable(L, klass) (Luna<klass>::RegisterMetatable((L)))
#define luna_inject(L, klass, t) (Luna<klass>::inject((L), (t)))

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

/**
A template class that offers convenient C++ LUA binding with very little code.
 \author nornagon
 \param T	The class that is to be wrapped for LUA
 \ingroup LUA
*/
template<class T> class Luna {
  public:
	/**
	Registers a C++ class, so that it can be instanciated and used from LUA.
	 \param L	A LUA state
	*/
    static void Register(lua_State *L) {
      lua_pushcfunction(L, &Luna<T>::constructor);
      lua_setglobal(L, T::className); // T() in lua will make a new instance.

      RegisterMetatable(L);
    }

    /**
	Register the metatable without registering the class constructor.
	Classes like this can be used if they are manualy instanciated. It is usualy
	not called from C++ directly, but as part of Register() or inject().
	 \param L	A LUA state
	*/
    static void RegisterMetatable(lua_State *L) {
      luaL_newmetatable(L, T::className); // create a metatable in the registry
      lua_pushstring(L, "__gc");
      lua_pushcfunction(L, &Luna<T>::gc_obj);
      lua_settable(L, -3); // metatable["__gc"] = Luna<T>::gc_obj
      lua_pop(L, 1);
    }

	/**
	The constructor function that is called by LUA. This method is registered
	as part of Register(). It creates a new instance of the class and pushes
	the object on the stack. It is usualy not called by users from C++.
	 \param L	A LUA state
	 \see	inject()
	 \see	Register()
	 \return the number of objects pushed on the stack, which is 1.
	*/
    static int constructor(lua_State *L) {
      return inject(L, new T(L));
    }

	/**
	Injects an existing instance into the passed LUA state. The object is pushed
	on the stack. The garbage collector method is not registered. If the class
	was registered already, instances injected will be collected, otherwise
	they are not.
	 \param L	A LUA stat
	 \param obj	The object instance to push on top of the stack
	 \return the number of objects pushed on the stack, which is 1.
	*/
    static int inject(lua_State *L, T* obj) {
      lua_newtable(L); // create a new table for the class object ('self')

      lua_pushnumber(L, 0);

      T** a = static_cast<T**>(lua_newuserdata(L, sizeof(T*))); // store a ptr to the ptr
      *a = obj; // set the ptr to the ptr to point to the ptr... >.>
      luaL_newmetatable(L, T::className); // get (or create) the unique metatable
      lua_setmetatable(L, -2); // self.metatable = uniqe_metatable

      lua_settable(L, -3); // self[0] = obj;

      for (int i = 0; T::Register[i].name; i++) { // register the functions
        lua_pushstring(L, T::Register[i].name);
        lua_pushnumber(L, i); // let the thunk know which method we mean
        lua_pushcclosure(L, &Luna<T>::thunk, 1);
        lua_settable(L, -3); // self["function"] = thunk("function")
      }
      return 1;
    }

	/**
	Returns an object that is on top of the stack. This method should only be called
	from a protected path. This is always the case in a LUA handler. Otherwise use
	lua_pcall() or lua_cpcall().
	 \param L	The lua state that contains the stack
	 \param idx	The index of the class that is to be extracted
	 \return The object that is on top of the stack.
	*/
	static T* extract(lua_State *L, int idx)
	{
		luaL_argcheck(L, lua_istable(L, idx), idx, "Not a Proxy Object");
		lua_pushnumber(L, 0);
		lua_gettable(L, idx); // get the class table (i.e, self)
		T** obj = static_cast<T**>(luaL_checkudata(L, -1, T::className));
		lua_remove(L, -1); // remove the userdata from the stack
		return *obj;
	}

	/**
	This method is registered to handle method calls. It looks up the correct
	function in the table of the class and calls it. It is not intended to be
	called directly from C++ code.
	 \param L	The lua state that is used in the function call. It contains the function arguments
	 \return	The number of values that were pushed on top of the stack by the function
	*/
    static int thunk(lua_State *L) {
      // redirect method call to the real thing
      int i = (int)lua_tonumber(L, lua_upvalueindex(1)); // which function?
      lua_pushnumber(L, 0);
      lua_gettable(L, 1); // get the class table (i.e, self)

      T** obj = static_cast<T**>(luaL_checkudata(L, -1, T::className));
      lua_remove(L, -1); // remove the userdata from the stack

      return ((*obj)->*(T::Register[i].mfunc))(L); // execute the thunk
    }

	/**
	The garbage collection function. It just deletes the instance, therefore freeing it's memory.
	This method is registered as lua function, and not intended to be called directly from C++.
	 \param L	The current lua state.
	 \return	The number 0
	*/
    static int gc_obj(lua_State *L) {
      // clean up
      //printf("GC called: %s\n", T::className);
      T** obj = static_cast<T**>(luaL_checkudata(L, -1, T::className));
      delete (*obj);
      return 0;
    }

	/**
	The type of the table that contains methods that are to be registered to LUA.
	Methods have to take only a LUA state as parameter, and return the number of
	values they pushed on top of the stack. Well behaved methods remove the
	arguments that are passed to them.
	*/
    struct RegType {
      const char *name; /**< The name of the method as seen from LUA */
      int(T::*mfunc)(lua_State*); /**< The method pointer */
    };
};


#endif /* LUNA_H */

