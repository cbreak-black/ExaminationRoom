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

#include "objects/container.h"

#include "vec.h"

namespace Examination
{
	class Object;
	class GLWidget;
	class Camera;

/**
This class represents the scene. It contains references to objects.
While it is itself an object, it should NOT be used as such.

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
	std::tr1::shared_ptr<Camera> camera() const;

public: // BG Color
	/**
	Sets the background color of this scene. This is a wrapper for setBackgroundColor(Tool::Vec4f c)
	 \param r	Red component
	 \param g	Green component
	 \param b	Blue component
	 \param a	Alpha component
	*/
	void setBackgroundColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	/**
	Sets the background color of this scene.
	 \param c	A color vector. Each component should be in the range of [0.0f, 1.0f]
	*/
	void setBackgroundColor(Tool::Color4 c);

	/**
	Returns the current background color of this scene.
	 \return The background color of this scene as vector. Each component should be in the range of [0.0f, 1.0f]
	*/
	Tool::Color4 backgroundColor() const;

protected:
	virtual Container * getParent();
	virtual Scene * getScene();

private:
	std::tr1::shared_ptr<Camera> camera_;
};

}

#endif
