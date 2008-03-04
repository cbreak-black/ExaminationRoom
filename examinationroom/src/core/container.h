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
	bool addObject(std::tr1::shared_ptr<Object> object);

	/**
	Removes an object from the container.
	 \param object	A pointer to an object
	*/
	void removeObject(std::tr1::shared_ptr<Object> object);

	/**
	Removes all objects.
	*/
	void clear();

public:
	virtual void draw(GLWidget * dest);

public:
	std::set< std::tr1::shared_ptr<Object> > objects();

private:
	std::set< std::tr1::shared_ptr<Object> > objects_;	
};

}
