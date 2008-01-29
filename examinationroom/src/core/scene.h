/*
 *  scene.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef SCENE_H
#define SCENE_H

#include <set>
#include <memory>

#include "drawing.h"

namespace Examination
{
	
class Object;
class GLWidget;

/**
This class represents the scene. It contains references to objects.

\author Gerhard Roethlin
*/
class Scene
{

public:
	/**
	Creator of scenes.
	*/
    Scene();

	/**
	Destructor of scenes. Also deletes all contained objects.
	*/
	~Scene();

public: // Drawing
	/**
	Draw this object for the side s.
	 \param s	the side to draw for.
	*/
	void drawScene(GLWidget * dest);
	
public: // Scene management
	/**
	Adds an object to the scene. An object can only be added once. The scene takes
	responsibility to free the used memory once it is not used anymore.
	 \param object	A pointer to an object
	 \return	true if the object was inserted, false if it already was in the scene.
	*/
	bool addObject(std::tr1::shared_ptr<Object> object);
	
	/**
	Removes an object from the scene and frees the memory it used. The object is deleted.
	 \param object	A pointer to an object
	*/
	void removeObject(std::tr1::shared_ptr<Object> object);
	
	/**
	Removes all objects.
	*/
	void clear();

private:
	std::set< std::tr1::shared_ptr<Object> > objects_;

};

}

#endif
