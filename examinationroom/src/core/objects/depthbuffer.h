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

#include "object.h"
#include "container.h"

namespace Examination
{

/**
This class represents a group of objects, which are drawn with their own depth
buffer state. This allows the manual ordering of drawing, which can lead to incorrect
occlusion.
*/
class DepthBuffer : public Container
{
public: // Construction
	DepthBuffer();

public:
	/**
	Returns the depth buffer state. True enables the depth buffer for all contained
	objects. False (the default) disables the depth buffer.
	 \return true if the depth buffer is enabled, false otherwise
	*/
	bool depthBufferState() const;

	/**
	Sets the depth buffer state. True enables the depth buffer for all contained
	objects. False (the default) disables the depth buffer.
	 \param enabled	The new depth buffer state
	*/
	void setDepthBufferState(bool enabled);

public:
	/**
	Draws all contained objects with the given depth buffer state.
	 \warn Drawing this node clears the depth buffer
	*/
	virtual void draw(GLWidget * dest) const;

private: // Depth buffer state
	bool depthBufferState_;

};

}

#endif
