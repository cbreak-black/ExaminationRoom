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
	class Scene;
	class Container;
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
	virtual Tool::Point position() const;

	/**
	Sets the position of the object.
	 \param p	the new position of the object.
	*/
	virtual void setPosition(Tool::Point p);

public: // Drawing
	virtual void draw(GLWidget * dest) const = 0;

	/**
	Sets the texture for this object.
	 \param t	Texture for left camera
	*/
	void setTexture(std::tr1::shared_ptr<AbstractTexture> t);

	/**
	Returns the texture of this object.
	\return	Texture of this object
	*/
	std::tr1::shared_ptr<AbstractTexture> texture() const;

public: // Color & Wireframe
	/**
	 Returns the color of this object. The color is mixed with the texture.
	 \return The color as 4 element vector. Each component is in the range [0.0, 1.0].
	 */
	Tool::Color4 color() const;

	/**
	 Sets the color of this object. The color is mixed with the texture.
	 \param color	The color as 4 element vector. Each component is in the range [0.0, 1.0].
	 */
	void setColor(Tool::Color4 color);

	/**
	 Returns wether this object is rendered in Wireframe mode.
	 \return	True if rendered in wirefrae, false if rendered solid
	 */
	bool wireframe() const;

	/**
	 Sets wether this object is rendered in Wireframe mode.
	 \param flag	True to render in wirefrae, false to render solid
	 */
	void setWireframe(bool flag);

public: // Nesting
	Scene * scene() const;
	virtual void setScene(Scene * s);
	Container * parent() const;
	virtual void setParent(Container * c);

private:
	Scene * scene_;
	Container * parent_;

	std::tr1::shared_ptr<AbstractTexture> tex_;
	Tool::Point origin_;
	Tool::Color4 color_;
	bool wireframe_;
};

}

#endif
