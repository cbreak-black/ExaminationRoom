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

namespace Examination
{
	class Object;
	class GLWidget;
	class Camera;

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
	Adds an object to the scene. An object can only be added once.
	 \param object	A shared pointer to an object
	 \return true if the object was inserted, false if it already was in the scene.
	*/
	bool addObject(std::tr1::shared_ptr<Object> object);
	
	/**
	Removes an object from the scene.
	 \param object	A pointer to an object
	*/
	void removeObject(std::tr1::shared_ptr<Object> object);
	
	/**
	Removes all objects.
	*/
	void clear();

public: // Camera
	void setCamera(std::tr1::shared_ptr<Camera> camera);
	std::tr1::shared_ptr<Camera> camera();

private:
	std::set< std::tr1::shared_ptr<Object> > objects_;
	std::tr1::shared_ptr<Camera> camera_;
};

}

#endif
