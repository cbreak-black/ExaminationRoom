/*
 *  container.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include <memory>
#include <set>

namespace Examination
{
	class Object;
	class Scene;
	class GLWidget;

class Container
{
public:
	Container();
	virtual ~Container();
	
public:
	/**
	Adds an object to the container. An object can only be added once.
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

protected:
	virtual Container * getParent() = 0;
	virtual Scene * getScene() = 0;
	void setParentsAndScenes(Container * p, Scene * s);

public:
	virtual void draw(GLWidget * dest) const;

public:
	std::set< std::tr1::shared_ptr<Object> > objects();

private:
	std::set< std::tr1::shared_ptr<Object> > objects_;	
};

}
