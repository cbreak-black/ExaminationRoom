#ifndef SCREEN_PROJECT_H
#define SCREEN_PROJECT_H

#include "vec.h"

#include <qgl.h>

namespace Tool
{

/**
Offers fast and easy to use projections from and to screen space. This class also caches
the product of the modelview and projection matrix, and it's inverse. Calculating the projection
of many points is accelerated, but be sure to call calculateMVP before every usage of the
projection methods.
*/
class ScreenProject
{

public: // ScreenSpace
	/**
	Precalculates the product of the Modelview and the Projection Matrix.
	This should only be done once every animation step, since it is unlikely
	to change during a frame.
	Call this function every time the Modelview Matrix, Projection Matrix, or
	the viewport changes, before calling a transformation function.
	 \param vp	Viewport information
	 \param mv	Modelview matrix
	 \param p	Projection matrix
	*/
	void calculateMVP(GLint * vp, double * mv, double * p);
	void calculateMVP();

	/**
	Transforms the given point from world space into screen space. Make sure to
	call calculateMVP or this call will fail silently.
	 \param _p The point in World Space
	 \param _f The fourth coordinate of a point, if desired
	 \return the transformed _p
	*/
	Point transformToScreenSpace(Point _p, float _f = 1) const;

	/**
	Transforms the given point from world space into clip space. Make sure to
	call calculateMVP or this call will fail silently.
	 \param _p The point in World Space
	 \param _f The fourth coordinate of a point, if desired
	 \return the transformed _p
	 */
	Point transformToClipSpace(Point _p, float _f = 1) const;

	/**
	Transforms the given point from screen space into world space. Make sure to
	call calculateMVP or this call will fail in unpredictable ways.
	 \param _p The point in screen space
	 \return the back transformed _p
	*/
	Point transformToWorldSpace(Point _p) const;

public:
	long * viewport();
	double * modelviewProjection();
	double * modelviewProjectionInv();

private:
	/**
	Helper. Inverts the matrix. Copied from Mesa.
	 \param m  input matrix
	 \param out  output matrix
	*/
	bool invertMatrix(const double * m, double * out) const;
	
	/**
	Helper. Transforms a Point with a matrix in OpenGL style. The fourth coordinate
	can be passed to transform the point in homogenous coordinates. The default is 1,
	which is reasonable to transform points. To transform vectors, pass a fourth
	coordinate of 0.
	 \param _p A Point
	 \param _m A pointer to 16 doubles
	 \param _f The fourth coordinate of a point, if desired
	 \return The Point transformed with _m
	*/
	inline Point transformPointWithMatrix(Point _p, const double * _m, float _f = 1) const;
	
private:
	double mvp_[16];	/**< Product of Modelview and Projection Matrix */
	double mvpInv_[16];	/**< Inverse of mvp_ */
	long vp_[4];		/**< Viewport */
};

}

#endif

