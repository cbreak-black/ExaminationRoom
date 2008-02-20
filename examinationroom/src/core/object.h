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

namespace Examination
{
	class GLWidget;
	class AbstractTexture;

/**
A generic object, which can be placed in a scene.

\author Gerhard Roethlin
*/
class Object
{

public:
	/**
	Creator of Objects.
	*/
    Object();
    
	/**
	Creator of Objects.
	\param x	X Coordinate of the object
	\param y	Y Coordinate of the object
	\param z	Z Coordinate of the object
	*/
    Object(float x, float y, float z);

	/**
	Creator of Objects.
	 \param o	Origin  of the object
	 */
    Object(Tool::Point o);

	/**
	Destructor of Objects
	*/
	virtual ~Object();

public: // Accessors
	/**
	Returns the position of this object.
	 \return the position of this object.
	*/
	Tool::Point position();

	/**
	Sets the position of the object.
	 \param p	the new position of the object.
	*/
	void setPosition(Tool::Point p);

	/**
	Sets the auto resize status. Auto resizing automatically scales
	textures to correspond pixel-by-pixel to the rendered size.
	 \param b	Boolean indicating the desired auto resize state
	*/
	void setAutoResize(bool b);
	
	/**
	Returns the auto resize status.
	 \return the auto resize status
	*/
	bool autoResize();

public: // Drawing
	virtual void draw(GLWidget * dest) = 0;

	/**
	Sets the texture for this object.
	 \param t	Texture for left camera
	*/
	void setTexture(std::tr1::shared_ptr<AbstractTexture> t);

	/**
	Returns the texture of this object.
	\return	Texture of this object
	*/
	std::tr1::shared_ptr<AbstractTexture> texture();

private:
	Tool::Point origin_;
	bool autoresize_;
	std::tr1::shared_ptr<AbstractTexture> tex_;
};

}

#endif
