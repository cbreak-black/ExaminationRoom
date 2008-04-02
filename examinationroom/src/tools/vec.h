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

#include "platform_math.h"

namespace Tool
{

/**
A small helper object, that is a 2 element vector. It can be treated as point
(with x, y accessors) or an array (with operator[] accessor).
*/
template <typename T>
union Vec2
{
	struct
	{
		T x;
		T y;
	};

	T vec[2];

	Vec2() {x = y = 0; };
	Vec2(T a, T b)
	{
		x = a;
		y = b;
	};

	/** Normalizes the vector */
	inline void normalize()
	{
		float dim = sqrt((*this)*(*this));
		x /= dim;
		y /= dim;
	};

	inline T& operator[](int i)
	{
		return vec[i];
	}
	inline const T& operator[](int i) const
	{
		return vec[i];
	}
};

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

	/** Normalizes the vector */
	inline void normalize()
	{
		float dim = sqrt((*this)*(*this));
		x /= dim;
		y /= dim;
		z /= dim;
	};

	inline T& operator[](int i)
	{
		return vec[i];
	}
	inline const T& operator[](int i) const
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

	/** Normalizes the vector */
	inline void normalize()
	{
		float dim = sqrt((*this)*(*this));
		x /= dim;
		y /= dim;
		z /= dim;
		t /= dim;
	};

	inline T& operator[](int i)
	{
		return vec[i];
	}
	inline const T& operator[](int i) const
	{
		return vec[i];
	}
};

typedef Vec2<float> Vec2f;
typedef Vec3<float> Vec3f;
typedef Vec4<float> Vec4f;
typedef Vec3f Point;
typedef Vec3f Vector;
typedef Vec3f Color3;
typedef Vec4f Color4;

// Operators Vec2f
template <typename T>
inline Vec2<T> & operator+=(Vec2<T> &v1, const Vec2<T> &v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	return v1;
}

template <typename T>
inline Vec2<T> & operator-=(Vec2<T> &v1, const Vec2<T> &v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	return v1;
}

template <typename T>
inline Vec2<T> & operator*=(Vec2<T> &v1, const T s1)
{
	v1.x *= s1;
	v1.y *= s1;
	return v1;
}

template <typename T>
inline Vec2<T> & operator/=(Vec2<T> &v1, const T s1)
{
	v1.x /= s1;
	v1.y /= s1;
	return v1;
}

template <typename T>
inline const Vec2<T> operator+(const Vec2<T> &v1, const Vec2<T> &v2)
{
	Vec2<T> v = v1;
	return v += v2;
}

template <typename T>
inline const Vec2<T> operator-(const Vec2<T> &v1, const Vec2<T> &v2)
{
	Vec2<T> v = v1;
	return v -= v2;
}

template <typename T>
inline const Vec2<T> operator*(const Vec2<T> &v1, const T s1)
{
	Vec2<T> v = v1;
	return v *= s1;
}

template <typename T>
inline const Vec2<T> operator*(const T s1, const Vec2<T> &v1)
{
	Vec2<T> v = v1;
	return v *= s1;
}

template <typename T>
inline const Vec2<T> operator/(const Vec2<T> &v1, const T s1)
{
	Vec2<T> v = v1;
	return v /= s1;
}

template <typename T>
inline const T operator*(const Vec2<T> &v1, const Vec2<T> &v2)
{
	return
		v1.x * v2.x +
		v1.y * v2.y;
}

// Operators Vec3f
template <typename T>
inline Vec3<T> & operator+=(Vec3<T> &v1, const Vec3<T> &v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

template <typename T>
inline Vec3<T> & operator-=(Vec3<T> &v1, const Vec3<T> &v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}

template <typename T>
inline Vec3<T> & operator*=(Vec3<T> &v1, const T s1)
{
	v1.x *= s1;
	v1.y *= s1;
	v1.z *= s1;
	return v1;
}

template <typename T>
inline Vec3<T> & operator/=(Vec3<T> &v1, const T s1)
{
	v1.x /= s1;
	v1.y /= s1;
	v1.z /= s1;
	return v1;
}

template <typename T>
inline const Vec3<T> operator+(const Vec3<T> &v1, const Vec3<T> &v2)
{
	Vec3<T> v = v1;
	return v += v2;
}

template <typename T>
inline const Vec3<T> operator-(const Vec3<T> &v1, const Vec3<T> &v2)
{
	Vec3<T> v = v1;
	return v -= v2;
}

template <typename T>
inline const Vec3<T> operator*(const Vec3<T> &v1, const T s1)
{
	Vec3<T> v = v1;
	return v *= s1;
}

template <typename T>
inline const Vec3<T> operator*(const T s1, const Vec3<T> &v1)
{
	Vec3<T> v = v1;
	return v *= s1;
}

template <typename T>
inline const Vec3<T> operator/(const Vec3<T> &v1, const T s1)
{
	Vec3<T> v = v1;
	return v /= s1;
}

template <typename T>
inline const T operator*(const Vec3<T> &v1, const Vec3<T> &v2)
{
	return
		v1.x * v2.x +
		v1.y * v2.y +
		v1.z * v2.z;
}

/** Calculates the cross product */
template <typename T>
inline const Vec3<T> cross(const Vec3<T> &v1, const Vec3<T> &v2)
{
	Vec3<T> v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

// Operators Vec4f
template <typename T>
inline Vec4<T> & operator+=(Vec4<T> &v1, const Vec4<T> &v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	v1.t += v2.t;
	return v1;
}

template <typename T>
inline Vec4<T> & operator-=(Vec4<T> &v1, const Vec4<T> &v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	v1.t -= v2.t;
	return v1;
}

template <typename T>
inline Vec4<T> & operator*=(Vec4<T> &v1, const T s1)
{
	v1.x *= s1;
	v1.y *= s1;
	v1.z *= s1;
	v1.t *= s1;
	return v1;
}

template <typename T>
inline Vec4<T> & operator/=(Vec4<T> &v1, const T s1)
{
	v1.x /= s1;
	v1.y /= s1;
	v1.z /= s1;
	v1.t /= s1;
	return v1;
}

template <typename T>
inline const Vec4<T> operator+(const Vec4<T> &v1, const Vec4<T> &v2)
{
	Vec4<T> v = v1;
	return v += v2;
}

template <typename T>
inline const Vec4<T> operator-(const Vec4<T> &v1, const Vec4<T> &v2)
{
	Vec4<T> v = v1;
	return v -= v2;
}

template <typename T>
inline const Vec4<T> operator*(const Vec4<T> &v1, const T s1)
{
	Vec4<T> v = v1;
	return v *= s1;
}

template <typename T>
inline const Vec4<T> operator*(const T s1, const Vec4<T> &v1)
{
	Vec4<T> v = v1;
	return v *= s1;
}

template <typename T>
inline const Vec4<T> operator/(const Vec4<T> &v1, const T s1)
{
	Vec4<T> v = v1;
	return v /= s1;
}

template <typename T>
inline const T operator*(const Vec4<T> &v1, const Vec4<T> &v2)
{
	return
		v1.x * v2.x +
		v1.y * v2.y +
		v1.z * v2.z +
		v1.t * v2.t;
}

}
#endif
