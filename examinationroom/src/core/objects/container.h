/*
 *  container.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef CONTAINER_H
#define CONTAINER_H

#include "object.h"

namespace Examination
{
	class Scene;
	class GLWidget;

/**
This class is a container object. It can store and manage other objects.
 \author Gerhard Roethlin
 \ingroup Objects
*/
class Container : public Object
{
public: // Types
	/**
	The type of the internal data structure that is used to contain objects.
	The most common operations is iterating over all objects.
	*/
	typedef std::list< std::tr1::shared_ptr<Object> > ObjectList;

public:
	/**
	Creates a default container.
	 \warning Don't create instances of this class directly, use Object::Create()
	 \see Object::Create()
	*/
	Container();

public:
	/**
	Destroys the container.
	All contained objects will be cleared, so if they are not referenced by an other
	shared_ptr, they are deleted.
	*/
	virtual ~Container();
	
public:
	/**
	Adds an object to the container. An object can only be added once.
	This method also removes the object from it's old container, if there was one.
	 \param object	A shared pointer to an object
	 \return true if the object was inserted, false if it already was in the scene.
	*/
	virtual bool addObject(std::tr1::shared_ptr<Object> object);

	/**
	Removes an object from the container.
	 \param object	A pointer to an object
	*/
	virtual void removeObject(std::tr1::shared_ptr<Object> object);

	/**
	Removes all objects.
	*/
	virtual void clear();

	/**
	Sorts the list of contained objects according their prefered draw ordering.
	This method is called by objects on their parent, when their drawPriority changes.
	*/
	void sortObjects();

protected:
	/**
	Returns the container that is set as parent of all objects that are added.
	The default implementation returns a pointer to this.
	 \return The container that serves as parent for all the objects contained in this container
	*/
	virtual std::tr1::shared_ptr<Container> getParent();

	/**
	Returns the scene this container is associated with
	*/
	virtual std::tr1::shared_ptr<Scene> getScene();

	/**
	Sets the parent and scene pointers of all direct children.
	 \param p	the new parent
	 \param s	the new scene
	*/
	void setParentsAndScenes(std::tr1::shared_ptr<Container> p, std::tr1::shared_ptr<Scene> s);

public: // From Object
	virtual void setScene(std::tr1::shared_ptr<Scene> s);

public:
	/**
	Draw the container, while applying it's function. Containers modify their contents
	in specific ways.
	 \param dest	The destination widget
	*/
	virtual void draw(GLWidget * dest) const;

	/**
	Returns true if the container is enabled.
	Being enabled means to apply the transformation of the container to the subobjects.
	When not enabled, the subobjects are drawn without change.
	The default is enabled.
	 \return the enable state
	*/
	bool enabled() const;

	/**
	Controlls if the container applies it's transformation.
	 \param enabled	True to enable the container, false to disable it.
	*/
	void setEnabled(bool enabled);

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

public: // LUA API
	static void registerLuaApi(luabridge::module * m);

public:
	/**
	Returns the internal object list.
	It should only be used to access individual objects, not to change the list itself.
	 \return ObjectList of all contained objects
	*/
	const ObjectList & objects() const;

protected:
	/**
	Calls layoutWillChange on the scene that contains this container, if it exists.
	*/
	void layoutWillChange() const;
	/**
	Calls layoutDidChange on the scene that contains this container, if it exists.
	*/
	void layoutDidChange() const;

private:
	ObjectList objects_;
	bool enabled_;

public: // Meta
	static const char * className_;
};

}

#endif
