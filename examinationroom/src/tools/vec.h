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

/**
A small helper object, that is a 3 element vector. It can be treated as point
(with x, y, z accessors), a color (with r, g, b accessors) or an array (with
operator[] accessor).
*/
template <typename T>
union Vec3
{
	struct
	{
		T x;
		T y;
		T z;
	};

	struct
	{
		T r;
		T g;
		T b;
	};
	
	T vec[3];
	
	Vec3() {x = y = z = 0; };
	Vec3(T a, T b, T c)
	{
		x = a;
		y = b;
		z = c;
	};

	inline T& operator[](int i)
	{
		return vec[i];
	}
	
};

/**
A small helper object, that is a 4 element vector. It can be treated as point
(with x, y, z, t accessors), a color (with r, g, b, a accessors) or an array (with
operator[] accessor).
*/
template <typename T> union Vec4
{
	struct
	{
		T x;
		T y;
		T z;
		T t;
	};

	struct
	{
		T r;
		T g;
		T b;
		T a;
	};
	
	T vec[4];

	Vec4() {x = y = z = t = 0; };
	Vec4(T a, T b, T c, T d)
	{
		x = a;
		y = b;
		z = c;
		t = d;
	};
	
	inline T& operator[](int i)
	{
		return vec[i];
	}	
	
};

typedef Vec3<float> Vec3f;
typedef Vec4<float> Vec4f;
typedef Vec3f Point;
typedef Vec3f Vector;

// Operators Vec3f

template <typename T>
inline Vec3<T> operator+(const Vec3<T> v1, const Vec3<T> v2)
{
	Vec3f v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	return v;
}

template <typename T>
inline Vec3<T> operator-(const Vec3<T> v1, const Vec3<T> v2)
{
	Vec3f v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	return v;
}

template <typename T>
inline Vec3<T> operator*(const Vec3<T> v1, const T s1)
{
	Vec3<T> v;
	v.x = v1.x * s1;
	v.y = v1.y * s1;
	v.z = v1.z * s1;
	return v;
}

template <typename T>
inline Vec3<T> operator*(const T s1, const Vec3<T> v1)
{
	Vec3<T> v;
	v.x = v1.x * s1;
	v.y = v1.y * s1;
	v.z = v1.z * s1;
	return v;
}

template <typename T>
inline T operator*(const Vec3<T> v1, const Vec3<T> v2)
{
	return
		v1.x * v2.x +
		v1.y * v2.y +
		v1.z * v2.z;
}

// Operators Vec4f

template <typename T>
inline Vec4<T> operator+(const Vec4<T> v1, const Vec4<T> v2)
{
	Vec4<T> v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	v.t = v1.t + v2.t;
	return v;
}

template <typename T>
inline Vec4<T> operator-(const Vec4<T> v1, const Vec4<T> v2)
{
	Vec4f v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	v.t = v1.t - v2.t;
	return v;
}

template <typename T>
inline Vec4<T> operator*(const Vec4<T> v1, const T s1)
{
	Vec4f v;
	v.x = v1.x * s1;
	v.y = v1.y * s1;
	v.z = v1.z * s1;
	v.t = v1.t * s1;
	return v;
}

template <typename T>
inline Vec4<T> operator*(const T s1, const Vec4<T> v1)
{
	Vec4f v;
	v.x = v1.x * s1;
	v.y = v1.y * s1;
	v.z = v1.z * s1;
	v.t = v1.t * s1;
	return v;
}

template <typename T>
inline T operator*(const Vec4<T> v1, const Vec4<T> v2)
{
	return
		v1.x * v2.x +
		v1.y * v2.y +
		v1.z * v2.z +
		v1.t * v2.t;
}

}
#endif
