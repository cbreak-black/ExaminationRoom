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

namespace Examination
{
	class GLWidget;
	class Scene;
	class Container;
	class AbstractTexture;
	class ParameterObject;

/**
\defgroup Objects	Scene Contents
Objects are the basis of a scene. They give it visual representation, which is ultimately
the goal of this tool. The object class and it's children can be inserted in a scene hierarchy
and positioned in space.
*/

/**
A generic object, which can be placed in a scene.
Everything in a scene is a subclass of this class.

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

public:
	/**
	Destructor of Objects
	*/
	virtual ~Object();

public: // Shared Pointers
	/**
	Creates a new Object instance and returns a shared pointer to it.
	enable_shared_from_this also internaly stores a weak_ptr to this for later use.
	 \return A shared_ptr to the new object instance
	*/
	template <typename O>
	static std::tr1::shared_ptr<O> Create()
	{
		std::tr1::shared_ptr<O> sharedPtr(new O());
		return sharedPtr;
	}

	/**
	Returns the shared_ptr to this object.
	This method only works if the object was created with Object::Create(),
	which is recommended.
	If the shared_ptr could not be locked, an invalid shared_ptr is returned.
	To casat it to a subclass, use std::tr1::dynamic_pointer_cast()
	 \return A shared_ptr to this object
	*/
	std::tr1::shared_ptr<Object> sharedPtr();

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
	This pointer should not be stored externaly.
	 \todo Change to use std:tr1:weak_ptr if possible
	 \return a pointer to the scene of this object
	*/
	std::tr1::shared_ptr<Scene> scene() const;

	/**
	Sets the scene pointer of this object. This should only be called by the
	Container this object is stored in. The container is responsible for maintaining
	the pointer.
	This method unregisters the object name in the program associated with the old scene
	and registers it with the program associated with the new scene.
	 \todo Change to use std:tr1:weak_ptr if possible
	 \param s	Scene pointer
	*/
	virtual void setScene(std::tr1::shared_ptr<Scene> s);

	/**
	Returns a pointer to the parent container this object is in.
	This poionter should not be stored externaly.
	 \todo Change to use std:tr1:weak_ptr if possible
	 \return a pointer to the container of this object
	*/
	std::tr1::shared_ptr<Container> parent() const;

	/**
	Sets the parent pointer of this object. This should only be called by the Container
	this object is stored in. The container is responsible for mantaining the pointer.
	When a new container pointer is set, the object has to be removed from the old container.
	This ensures that it is both only stored once in a container (by removing itself even if it gets
	inserted into the same) and that it is only inserted in one container. Keeping the data structure
	in order is the responsibility of the container class.
	 \todo Change to use std:tr1:weak_ptr if possible
	 \param c	Container pointer
	*/
	virtual void setParent(std::tr1::shared_ptr<Container> c);

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

public: // Parameter Dialog
	/**
	Returns a ParameterDialog subclass instance associated with this object.
	If none is cached, it is created and returned, otherwise a cached instance
	is returned.
	*/
	std::tr1::shared_ptr<ParameterObject> dialog();

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
	 \param cb	A callback, created with std::tr1::bind
	*/
	void addCallbackParameterChanged(const SignalCallbackType & cb);

	/**
	Removes all callbacks with the passed target.
	 \param target	An object used as target of a callback
	*/
	template <typename T>
	void removeCallbackParameterChanged(const T * target)
	{
		removeCallbacks(parameterChanged_, target);
	}

protected: // Signals: Internal data and methods
	/**
	Helper Method: Calls all contained functionals with the passed object as argument.
	 \param list	A list of Signal callbacks
	 \param obj		A pointer to an object to pass to the callbacks, traditionaly "this"
	*/
	static void callCallbacks(const std::list<SignalCallbackType> & list, const Object * obj);

	/**
	Helper Method: Removes all callbacks with the given target.
	 \param list	A list of Signal callbacks
	 \param target	A reference wrapper to the target of the callbacks to be removed
	*/
	template <typename T>
	static void removeCallbacks(std::list<SignalCallbackType> & list,
								const T * target)
	{
		std::list<SignalCallbackType>::iterator it = list.begin();
		std::list<SignalCallbackType>::iterator end = list.end();
		for (; it != end; it++)
		{
			if (target == *it)
			{
				list.erase(it);
			}
		}
	}

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

private: // State (not exported)
	std::tr1::weak_ptr<Scene> scene_;
	std::tr1::weak_ptr<Container> parent_;

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
};

/**
Returns true if the drawing priority of a is smaller than b.
 \return true if the drawing priority of a is smaller than b.
*/
bool operator<(std::tr1::shared_ptr<Object> & a, std::tr1::shared_ptr<Object> & b);

}

#endif
