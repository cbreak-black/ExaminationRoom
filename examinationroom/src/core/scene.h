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

#include "container.h"

namespace Examination
{
	class Object;
	class GLWidget;
	class Camera;

/**
This class represents the scene. It contains references to objects.

\author Gerhard Roethlin
*/
class Scene : public Container
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

public: // Camera
	void setCamera(std::tr1::shared_ptr<Camera> camera);
	std::tr1::shared_ptr<Camera> camera();

private:
	std::tr1::shared_ptr<Camera> camera_;
};

}

#endif
