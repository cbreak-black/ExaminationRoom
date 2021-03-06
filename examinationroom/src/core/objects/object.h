/*
 *  object.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "vec.h"

#include <memory>
#include <string>
#include <list>
#include <set>
#include <functional>
#include <iostream>

namespace luabridge
{
	class module;
}

namespace Examination
{
	class GLWidget;
	class Scene;
	class Container;
	class AbstractTexture;
	class ParameterObject;
	class Object;

/**
The type of Object pointers.
Raw pointers should never be used.
*/
typedef std::tr1::shared_ptr<Object> ObjectPtr;

/**
The type of const Object pointers.
Raw pointers should never be used.
*/
typedef std::tr1::shared_ptr<const Object> ConstObjectPtr;

/**
\defgroup Objects	Scene Contents
Objects are the basis of a scene. They give it visual representation, which is ultimately
the goal of this tool. The object class and it's children can be inserted in a scene hierarchy
and positioned in space.
*/

/**
A generic object, which can be placed in a scene.
Everything in a scene is a subclass of this class.
This class should always be used wrapped in a shared_ptr, or some functions might not work,
such as sharedPtr(), which relies on enable_shared_from_this.
It should never be used by-value.

When subclassing, several methods have to be overwritten to provide a
consistent user experience. The draw() method is called every time the object
is drawn. The passed widget can be used to query a limited amount of information
or to perform specific tasks. Drawing is done with OpenGL calls.

The method clone() creates a clone of the called object. This happens in two steps:
The copy constructor Object(const Object&) creates a new object from a reference to an
object. It should be overwritten if reference parameters are used.
The clone() method uses this copy constructor to create a new instance of a subclass,
wrapps it into an ObjectPtr and returns it.

The method className() and the static variable className_ should both be changed
to reflect the new class name.

The method toLua() can be changed to write additional commands for added state.
Subclasses should always call the parent's toLua() method before writing their own
commands.

The method dialog() returns a class specific instance of a ParameterDialog subclass.
Subclasses can overwrite createDialog() to return their own instance. This instance
is then cached until the object is destroyed.

The method Object::registerLuaApi() is called by the Program in
Program::registerComponents() when adding new objects.
Subclasses of Object are required to overwrite this static method and register their
own methods with LUA.
Even when not adding new methods, the creator and the parent must still be registered.

 \author Gerhard Roethlin
 \ingroup Objects
*/
class Object : public std::tr1::enable_shared_from_this<Object>
{
public:
	/**
	Creator of Objects.
	*/
    Object();

	/**
	Creator of Objects.
	Initializes it with all relevant values of o.
	*/
	Object(const Object & o);

public:
	/**
	Destructor of Objects
	*/
	virtual ~Object();

public: // Cloning
	/**
	Returns a newly allocated clone of this object.
	All parameters of the clone are copied.
	The clone is not contained in a view hierarchy and has no registered
	callbacks.
	 \return a newly created copy of this object
	*/
	virtual std::tr1::shared_ptr<Object> clone() const = 0;

public: // Shared Pointers
	/**
	Returns the shared_ptr to this object.
	This method only works if the object is referenced by a shared_ptr,
	which is required for some functionality.
	If the shared_ptr could not be locked, an invalid shared_ptr is returned.
	To cast it to a subclass, use std::tr1::dynamic_pointer_cast()
	 \return A shared_ptr to this object
	*/
	ObjectPtr sharedPtr();

	/**
	 \overload
	*/
	ConstObjectPtr sharedPtr() const;

public: // Accessors
	/**
	Returns the position of this object.
	 \return the position of this object.
	*/
	virtual Tool::Point position() const;

	/**
	Sets the position of the object.
	 \param p	the new position of the object.
	*/
	virtual void setPosition(Tool::Point p);

public: // Drawing
	/**
	Draws the object. Implementators should check for "shown()" to return before
	drawing itself.
	 \param dest	The widget to draw into
	*/
	virtual void draw(GLWidget * dest) const = 0;

	/**
	Sets the texture for this object.
	 \param t	Texture for left camera
	*/
	virtual void setTexture(std::tr1::shared_ptr<AbstractTexture> t);

	/**
	Returns the texture of this object.
	\return	Texture of this object
	*/
	std::tr1::shared_ptr<AbstractTexture> texture() const;

public: // Color & Wireframe
	/**
	 Returns the color of this object. The color is mixed with the texture.
	 \return The color as 4 element vector. Each component is in the range [0.0, 1.0].
	 */
	Tool::Color4 color() const;

	/**
	 Sets the color of this object. The color is mixed with the texture.
	 \param color	The color as 4 element vector. Each component is in the range [0.0, 1.0].
	 */
	void setColor(Tool::Color4 color);

	/**
	 Returns wether this object is rendered in Wireframe mode.
	 \return	True if rendered in wirefrae, false if rendered solid
	 */
	bool wireframe() const;

	/**
	 Sets wether this object is rendered in Wireframe mode.
	 \param flag	True to render in wirefrae, false to render solid
	 */
	void setWireframe(bool flag);

public: // Visibility
	/**
	Returns if this object is shown, that is if it would be drawn when it's parent
	is drawn. The default is true.
	 \return true when the object is drawn, false when it's not
	*/
	bool shown() const;

	/**
	Sets the object to be shown or hidden.
	 \param shown	true to show the object, false to not show it
	*/
	void setShown(bool shown);

	/**
	 Returns if this object is visible. An object is visible if it's parent is visible
	 and it is shown.
	 \return the visibility of this object
	 */
	bool visible() const;

public: // Nesting
	/**
	Returns a pointer to the scene this object is in.
	 \return a pointer to the scene of this object
	*/
	virtual std::tr1::shared_ptr<Scene> scene();
	virtual std::tr1::shared_ptr<Scene const> scene() const;

	/**
	Sets the scene pointer of this object. This should only be called by the
	Container this object is stored in. The container is responsible for maintaining
	the pointer.
	This method unregisters the object name in the program associated with the old scene
	and registers it with the program associated with the new scene.
	This method can not be called on constant objects since it might change the name.
	 \param s	Scene pointer
	*/
	virtual void setScene(std::tr1::shared_ptr<Scene> s);

	/**
	Returns a pointer to the parent container this object is in.
	This poionter should not be stored externaly.
	 \return a pointer to the container of this object
	*/
	virtual std::tr1::shared_ptr<Container> parent() const;

	/**
	Sets the parent pointer of this object. This should only be called by the Container
	this object is stored in. The container is responsible for mantaining the pointer.
	When a new container pointer is set, the object has to be removed from the old container.
	This ensures that it is both only stored once in a container (by removing itself even if it gets
	inserted into the same) and that it is only inserted in one container. Keeping the data structure
	in order is the responsibility of the container class.
	 \param c	Container pointer
	*/
	virtual void setParent(std::tr1::shared_ptr<Container> c) const;

public: // Drawing priority
	/**
	Returns the drawing priority of this class. The default is 0. A positive number means prefering to
	be drawn late, negative numbers means to prefer to be drawn early.
	This method also re-sorts the parent container.
	 \return The draw priority of this object instance.
	*/
	int drawPriority() const;

	/**
	Sets the drawing priority of this class. Positive numbers give less priority, negative numbers more.
	 \param priority	The new priority.
	*/
	void setDrawPriority(int priority);

public: // Name
	/**
	Returns the name of this object.
	 \return the name of this object
	*/
	const std::string & name() const;

	/**
	Sets the name of this object. Note that the name might be changed to a derivate
	of the passed name to fulfill uniqueness constraints.
	 \note	Satisfies uniqueness constraints
	 \param name	The new name of this object
	 \return True if the name was set as passed, false if it had to be changed.
	*/
	bool setName(const std::string & name);

public: // Serialisation
	/**
	Returns the name of the class of this object. This can be used in LUA
	object creation.
	 \return The name of this object's class as c++ string
	*/
	virtual std::string className() const;

	/**
	Writes the LUA commands to set parameters of this object to the output stream.
	 \param outStream	A stream that accepts writing
	*/
	virtual std::string toLua(std::ostream & outStream) const;

	/**
	Writes the LUA creation command of this object to the output stream.
	 \param outStream	A stream that accepts writing
	*/
	virtual std::string toLuaCreate(std::ostream & outStream) const;

public: // LUA API
	/**
	This static method has to be reimplemented in all subclasses.
	It is responsible for registering the class' constructor, the parent class
	and the new methods with LUA.
	Registration is done with luabridge.
	 \param m	A pointer to a luabridge::module instance that acts on the intended lua_State
	*/
	static void registerLuaApi(luabridge::module * m);

public: // Parameter Dialog
	/**
	Returns a ParameterDialog subclass instance associated with this object.
	If none is cached, it is created and returned, otherwise a cached instance
	is returned.
	Does not exist in a constant variant, since changing parameters for
	constant objects does not make sense.
	 \return pointer to a ParameterDialog
	*/
	std::tr1::shared_ptr<ParameterObject> dialog();

protected:
	/**
	Creates a parameter dialog and stores it internally.
	Subclasses can overwrite this method to create their own instances.
	*/
	virtual std::tr1::shared_ptr<ParameterObject> createDialog();

public: // Signals
	/**
	The type of the callbacks. Use bind() to create.
	Do NOT rely on the Object * to stay valid after the callback returns, don't
	store it.
	*/
	typedef std::tr1::function<void (const Object *)> SignalCallbackType;

	/**
	This method adds a parameterChanged callback. It is called every time the
	object's parametrisation changes.
	Constant objects are not observable since they will not change anyway.
	 \param cb	A callback, created with std::tr1::bind
	*/
	void addCallbackParameterChanged(const SignalCallbackType & cb);

protected: // Signals: Internal data and methods
	/**
	Helper Method: Calls all contained functionals with the passed object as argument.
	 \param list	A list of Signal callbacks
	 \param obj		A pointer to an object to pass to the callbacks, traditionaly "this"
	*/
	static void callCallbacks(const std::list<SignalCallbackType> & list, const Object * obj);

protected: // Notify scene/observers of changes
	/**
	Calls objectWillChange on the scene that contains this object, if it exists.
	*/
	void objectWillChange() const;

	/**
	Calls objectDidChange on the scene that contains this object, if it exists.
	Also calls the callbacks registered with addCallbackParameterChanged.
	*/
	void objectDidChange() const;

private: // No-copy
	/**
	Does nothing, since objects are not asignable. Use clone() instead.
	*/
	Object & operator=(const Object &);

private: // State (not exported)
	mutable std::tr1::weak_ptr<Scene> scene_;
	mutable std::tr1::weak_ptr<Container> parent_;

	// List with callbacks
	std::list<SignalCallbackType> parameterChanged_;

	// Parameter Dialog
	std::tr1::shared_ptr<ParameterObject> dialog_;

private: // Parameters (exported)
	std::string name_;

	std::tr1::shared_ptr<AbstractTexture> tex_;
	Tool::Point origin_;
	Tool::Color4 color_;
	bool wireframe_;
	bool shown_;
	int drawPriority_;

public: // Meta
	/**
	This static member is used in registration of the class with the program.
	Subclasses are required to provide their own name the same way.
	*/
	static const char * className_;
};

/**
Returns true if the drawing priority of a is smaller than b.
 \return true if the drawing priority of a is smaller than b.
*/
bool operator<(ObjectPtr & a, ObjectPtr & b);

/**
Base class for object factories.
An object factory is an object that creates "Object" instances.
*/
class ObjectFactoryBase
{
public:
	virtual ~ObjectFactoryBase() {};
public:
	/**
	Creates a new instance of Object or a subclass and returns a shared_ptr to it
	 \return shared_ptr to the newly created Object
	*/
	virtual ObjectPtr create() const = 0;
	virtual std::string name() const = 0;
};

typedef std::tr1::shared_ptr<ObjectFactoryBase> ObjectFactoryPtr;

template <class O>
class ObjectFactory : public ObjectFactoryBase
{
public:
	virtual std::tr1::shared_ptr<Object> create() const
	{
		std::tr1::shared_ptr<O> sharedPtr(new O());
		return sharedPtr;
	}
	virtual std::string name() const
	{
		return O::className_;
	}
};

}

#endif
