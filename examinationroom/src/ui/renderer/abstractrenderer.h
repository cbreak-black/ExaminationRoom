/*
 *  abstractrenderer.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include <memory>

namespace Examination
{
	class Scene;
	class GLWidget;

class AbstractRenderer
{
public: // Creation
	/**
	Creates a new Renderer, and sets the scene reference
	*/
	AbstractRenderer(std::tr1::shared_ptr<Scene> scene);

	/**
	Destroys the renderer.
	Does nothing.
	*/
	virtual ~AbstractRenderer();

public: // Accessors
	/**
	Returns the scene of this renderer as shared_ptr.
	 \return The scene of this renderer
	*/
	std::tr1::shared_ptr<Scene> scene() const;

	/**
	Sets the scene of this renderer.
	 \param scene	the new scene
	*/
	void setScene(std::tr1::shared_ptr<Scene> scene);

public: // Drawing
	/**
	Called to render the scene.
	Subclasses are expected to implement this method.
	The passed GLWidget can be expected to refer the current opengl context.
	 \param w	The GLWidget to render into
	*/
	virtual void renderScene(GLWidget * w) = 0;

private:
	std::tr1::shared_ptr<Scene> scene_;
};

}

#endif
