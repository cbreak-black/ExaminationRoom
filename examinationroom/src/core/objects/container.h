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

#include <memory>
#include <list>

namespace Examination
{
	class Scene;
	class GLWidget;

/**
This class is a container object. It can store and manage other objects.
*/
class Container : public Object
{
public:
	Container();
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
	virtual Container * getParent();

	/**
	Returns the scene this container is associated with
	*/
	virtual Scene * getScene();

	/**
	Sets the parent and scene pointers of all direct children.
	 \param p	the new parent
	 \param s	the new scene
	*/
	void setParentsAndScenes(Container * p, Scene * s);

public:
	virtual void setScene(Scene * s);
	virtual void setParent(Container * c);

public:
	virtual void draw(GLWidget * dest) const;

public:
	const std::list< std::tr1::shared_ptr<Object> > objects();

private:
	std::list< std::tr1::shared_ptr<Object> > objects_;
};

}

#endif
