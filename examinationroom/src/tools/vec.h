/*
 *  vec.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef VEC_H
#define VEC_H

namespace Tool
{

typedef union _Vec3f
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
	
	_Vec3f() {x = y = z = 0; };
	_Vec3f(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	};

	inline float& operator[](int i)
	{
		return vec[i];
	}
	
} Vec3f;

typedef union _Vec4f
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

	_Vec4f() {x = y = z = t = 0; };
	_Vec4f(float a, float b, float c, float d)
	{
		x = a;
		y = b;
		z = c;
		t = d;
	};
	
	inline float& operator[](int i)
	{
		return vec[i];
	}	
	
} Vec4f;

typedef Vec3f Point;
typedef Vec3f Vector;

// Operators Vec3f

inline Vec3f operator+(const Vec3f v1, const Vec3f v2)
{
	Vec3f v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	return v;
}

inline Vec3f operator-(const Vec3f v1, const Vec3f v2)
{
	Vec3f v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	return v;
}

inline Vec3f operator*(const Vec3f v1, const float s1)
{
	Vec3f v;
	v.x = v1.x * s1;
	v.y = v1.y * s1;
	v.z = v1.z * s1;
	return v;
}

inline Vec3f operator*(const float s1, const Vec3f v1)
{
	Vec3f v;
	v.x = v1.x * s1;
	v.y = v1.y * s1;
	v.z = v1.z * s1;
	return v;
}

// Operators Vec4f

inline Vec4f operator+(const Vec4f v1, const Vec4f v2)
{
	Vec4f v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	v.t = v1.t + v2.t;
	return v;
}

inline Vec4f operator-(const Vec4f v1, const Vec4f v2)
{
	Vec4f v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	v.t = v1.t - v2.t;
	return v;
}

inline Vec4f operator*(const Vec4f v1, const float s1)
{
	Vec4f v;
	v.x = v1.x * s1;
	v.y = v1.y * s1;
	v.z = v1.z * s1;
	v.t = v1.t * s1;
	return v;
}

inline Vec4f operator*(const float s1, const Vec4f v1)
{
	Vec4f v;
	v.x = v1.x * s1;
	v.y = v1.y * s1;
	v.z = v1.z * s1;
	v.t = v1.t * s1;
	return v;
}

}
#endif
