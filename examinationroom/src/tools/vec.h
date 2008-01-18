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

#ifndef VEC_H
#define VEC_H

namespace tool
{

typedef union
{
	struct
	{
		float x;
		float y;
		float z;
	};

	struct
	{
		float r;
		float g;
		float b;
	};
	
	float vec[3];
	
} Vec3f;

typedef union
{
	struct
	{
		float x;
		float y;
		float z;
		float t;
	};

	struct
	{
		float r;
		float g;
		float b;
		float a;
	};
	
	float vec[4];
	
} Vec4f;

typedef Vec3f Point;
typedef Vec3f Vector;

typedef Vec3f Point;

}
#endif