/*******************************************************************************

Examination Room is a flexible user evaluation tool

Copyright (C) 2008 Gerhard Roethlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************************/

#ifndef SCENE_H
#define SCENE_H

typedef  enum {left = 1, right = 2} Side;

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
	
public:
	drawScene(Side s);

};

#endif
