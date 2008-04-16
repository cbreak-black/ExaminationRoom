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
#include <string>

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

private: // Initialisation
	/**
	Sets default values.
	*/
	void init();

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
	/**
	Draws the object. Implementators should check for "shown()" to return before
	drawing itself.
	 \param dest	The widget to draw into
	*/
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

public: // Visibility
	/**
	Returns if this object is shown, that is if it would be drawn when it's parent
	is drawn. The default is true.
	 \return true when the object is drawn, false when it's not
	*/
	bool shown() const;

	/**
	Sets the object to be shown or hidden.
	 \param shown	true to show the object, false to not show it
	*/
	void setShown(bool shown);

	/**
	 Returns if this object is visible. An object is visible if it's parent is visible
	 and it is shown.
	 \return the visibility of this object
	 */
	bool visible() const;

public: // Nesting
	/**
	Returns a pointer to the scene this object is in.
	This pointer should not be stored externaly.
	 \todo Change to use std:tr1:weak_ptr if possible
	 \return a pointer to the scene of this object
	*/
	Scene * scene() const;

	/**
	Sets the scene pointer of this object. This should only be called by the
	Container this object is stored in. The container is responsible for maintaining
	the pointer.
	 \todo Change to use std:tr1:weak_ptr if possible
	 \param s	Scene pointer
	*/
	virtual void setScene(Scene * s);

	/**
	Returns a pointer to the parent container this object is in.
	This poionter should not be stored externaly.
	 \todo Change to use std:tr1:weak_ptr if possible
	 \return a pointer to the container of this object
	*/
	Container * parent() const;

	/**
	Sets the parent pointer of this object. This should only be called by the Container
	this object is stored in. The container is responsible for mantaining the pointer.
	When a new container pointer is set, the object has to be removed from the old container.
	This ensures that it is both only stored once in a container (by removing itself even if it gets
	inserted into the same) and that it is only inserted in one container. Keeping the data structure
	in order is the responsibility of the container class.
	 \todo Change to use std:tr1:weak_ptr if possible
	 \param c	Container pointer
	*/
	virtual void setParent(Container * c);

public: // Drawing priority
	/**
	Returns the drawing priority of this class. The default is 0. A positive number means prefering to
	be drawn late, negative numbers means to prefer to be drawn early.
	This method also re-sorts the parent container.
	 \return The draw priority of this object instance.
	*/
	int drawPriority() const;

	/**
	Sets the drawing priority of this class. Positive numbers give less priority, negative numbers more.
	 \param priority	The new priority.
	*/
	void setDrawPriority(int priority);

public: // Name
	/**
	Returns the name of this object.
	 \return the name of this object
	*/
	const std::string & name() const;

	/**
	Sets the name of this object.
	 \param name	The new name of this object
	*/
	void setName(const std::string & name);

protected:
	/**
	Calls objectWillChange on the scene that contains this container, if it exists.
	*/
	void objectWillChange() const;

	/**
	Calls objectDidChange on the scene that contains this container, if it exists.
	*/
	void objectDidChange() const;

private:
	Scene * scene_;
	Container * parent_;

	std::string name_;

	std::tr1::shared_ptr<AbstractTexture> tex_;
	Tool::Point origin_;
	Tool::Color4 color_;
	bool wireframe_;
	bool shown_;
	int drawPriority_;
};

/**
Returns true if the drawing priority of a is smaller than b.
 \return true if the drawing priority of a is smaller than b.
*/
bool operator<(std::tr1::shared_ptr<Object> & a, std::tr1::shared_ptr<Object> & b);

}

#endif
