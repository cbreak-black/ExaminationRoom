/*
 *  depthbuffer.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.08.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef DEPTHBUFFER_NODE_H
#define DEPTHBUFFER_NODE_H

#include "container.h"

namespace Examination
{

/**
This class represents a group of objects, which are drawn with their own depth
buffer state. Also, after drawing each object, the depth buffer is reset.
This allows the manual ordering of drawing, which can lead to incorrect
occlusion.
 \author Gerhard Roethlin
 \ingroup Objects
*/
class DepthBuffer : public Container
{
public: // Construction
	/**
	Constructs a DepthBuffer object with default settings.
	The depht buffer is enabled by default, but is reset before each sub object
	is drawn.
	 \warning Don't create instances of this class directly, use Object::Create()
	 \see Object::Create()
	*/
	DepthBuffer();

public:
	/**
	Returns the depth buffer state. True (the default) enables the depth buffer
	for all contained objects. False disables the depth buffer.
	 \return true if the depth buffer is enabled, false otherwise
	*/
	bool depthBufferState() const;

	/**
	Sets the depth buffer state. True (the default) enables the depth buffer
	for all contained objects. False disables the depth buffer.
	 \param enabled	The new depth buffer state
	*/
	void setDepthBufferState(bool enabled);

public:
	/**
	Draws all contained objects with the given depth buffer state.
	Before each individual sub object, the depth buffer is cleared.
	 \warning Drawing this node clears the depth buffer
	*/
	virtual void draw(GLWidget * dest) const;

public: // Serialisation
	/**
	Returns the name of the class of this object. This can be used in LUA
	object creation.
	 \return The name of this object's class as c++ string
	*/
	virtual std::string className() const;

	/**
	Writes the LUA commands to set parameters of this object to the output stream.
	 \param outStream	A stream that accepts writing
	*/
	virtual std::string toLua(std::ostream & outStream) const;

private: // Depth buffer state
	bool depthBufferState_;

};

}

#endif
