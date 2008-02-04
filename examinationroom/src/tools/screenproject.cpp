#include "screenproject.h"

#include "platform_math.h"

#include <qgl.h>

namespace Tool
{

// Screen Space
void ScreenProject::calculateMVP(long * vp, double * mv, double * p)
{
	vp_[0] = vp[0];
	vp_[1] = vp[1];
	vp_[2] = vp[2];
	vp_[3] = vp[3];

	// Matrix Multiplication
	for (int k=0; k<16; k+=4)
	{
		for (int j=0; j<4; j++)
		{
			mvp_[j+k] = 0;
			for (int i=0; i<4; i++)
				mvp_[j+k] += p[4*i+j]*mv[i+k];
		}
	}
	
	invertMatrix(mvp_, mvpInv_);
}

void ScreenProject::calculateMVP()
{
	double mv[16];
	double p[16];
	long vp[4];
	
	calculateMVP(vp, mv, p);
}

Point ScreenProject::transformToScreenSpace(Point _p, float _f) const
{
	Point pT = transformPointWithMatrix(_p, mvp_, _f);
	
	return Point(
				 vp_[0] + vp_[2] * (pT[0] + 1)/2,
				 vp_[1] + vp_[3] * (pT[1] + 1)/2,
				 (pT[2] + 1)/2);
}

Point ScreenProject::transformToWorldSpace(Point _p) const
{
	// Transform to normalized coordinates
	_p[0] = (_p[0] - vp_[0]) * 2 / vp_[2] - 1.0f;
	_p[1] = (_p[1] - vp_[1]) * 2 / vp_[3] - 1.0f;
	_p[2] = 2 * _p[2] - 1.0;
	
	// Transform
	return transformPointWithMatrix(_p, mvpInv_);
}

Point ScreenProject::transformPointWithMatrix(Point _p, const double * _m, float _f) const
{
    float xp = _m[0] * _p[0] + _m[4] * _p[1] + _m[8] * _p[2] + _f * _m[12];
    float yp = _m[1] * _p[0] + _m[5] * _p[1] + _m[9] * _p[2] + _f * _m[13];
    float zp = _m[2] * _p[0] + _m[6] * _p[1] + _m[10] * _p[2] + _f * _m[14];
    float tp = _m[3] * _p[0] + _m[7] * _p[1] + _m[11] * _p[2] + _f * _m[15];
    if (tp == 0)
		return Point(xp, yp, zp);
    else
		return Point(xp / tp, yp / tp, zp / tp);
}

bool ScreenProject::invertMatrix(const double * m, double * out) const
{
	/*
	 * Compute inverse of 4x4 transformation matrix.
	 * Code contributed by Jacques Leroy jle@star.be
	 * Return GL_TRUE for success, GL_FALSE for failure (singular matrix)
	 */
	// http://www.mesa3d.org/license.html
	// http://webcvs.freedesktop.org/mesa/Mesa/src/glu/mesa/project.c?content-type=text%2Fplain&view=co
	//static GLboolean invert_matrix(const GLdouble * m, GLdouble * out)
	/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { GLdouble *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]
	
	double wtmp[4][8];
	double m0, m1, m2, m3, s;
	double *r0, *r1, *r2, *r3;
	
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	
	r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
		r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
		r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
		r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
		r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
		r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
		r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
		r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
	
	/* choose pivot - or die */
	if (fabs(r3[0]) > fabs(r2[0]))
		SWAP_ROWS(r3, r2);
	if (fabs(r2[0]) > fabs(r1[0]))
		SWAP_ROWS(r2, r1);
	if (fabs(r1[0]) > fabs(r0[0]))
		SWAP_ROWS(r1, r0);
	if (0.0 == r0[0])
		return false;
	
	/* eliminate first variable     */
	m1 = r1[0] / r0[0];
	m2 = r2[0] / r0[0];
	m3 = r3[0] / r0[0];
	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;
	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;
	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) {
		r1[4] -= m1 * s;
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r0[5];
	if (s != 0.0) {
		r1[5] -= m1 * s;
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r0[6];
	if (s != 0.0) {
		r1[6] -= m1 * s;
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r0[7];
	if (s != 0.0) {
		r1[7] -= m1 * s;
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	
	/* choose pivot - or die */
	if (fabs(r3[1]) > fabs(r2[1]))
		SWAP_ROWS(r3, r2);
	if (fabs(r2[1]) > fabs(r1[1]))
		SWAP_ROWS(r2, r1);
	if (0.0 == r1[1])
		return false;
	
	/* eliminate second variable */
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];
	s = r1[4];
	if (0.0 != s) {
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r1[5];
	if (0.0 != s) {
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r1[6];
	if (0.0 != s) {
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r1[7];
	if (0.0 != s) {
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	
	/* choose pivot - or die */
	if (fabs(r3[2]) > fabs(r2[2]))
		SWAP_ROWS(r3, r2);
	if (0.0 == r2[2])
		return false;
	
	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
	
	/* last check */
	if (0.0 == r3[3])
		return false;
	
	s = 1.0 / r3[3];    /* now back substitute row 3 */
	r3[4] *= s;
	r3[5] *= s;
	r3[6] *= s;
	r3[7] *= s;
	
	m2 = r2[3];     /* now back substitute row 2 */
	s = 1.0 / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
	
	m1 = r1[2];     /* now back substitute row 1 */
	s = 1.0 / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
	
	m0 = r0[1];     /* now back substitute row 0 */
	s = 1.0 / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
	
	MAT(out, 0, 0) = r0[4];
	MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
	MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
	MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
	MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
	MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
	MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
	MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
	MAT(out, 3, 3) = r3[7];
	
	return true;
	
#undef MAT
#undef SWAP_ROWS
}

}