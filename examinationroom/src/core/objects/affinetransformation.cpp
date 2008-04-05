/*
 *  affinetransformation.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "affinetransformation.h"

#include <qgl.h>

#include "platform_math.h"

using namespace std::tr1;

namespace Examination
{
	using namespace Tool;
	
AffineTransformation::AffineTransformation()
{
	loadIdentity();
}

void AffineTransformation::loadIdentity()
{
	trans_[0] = 1;	trans_[4] = 0;	trans_[8] = 0;	trans_[12] = 0;
	trans_[1] = 0;	trans_[5] = 1;	trans_[9] = 0;	trans_[13] = 0;
	trans_[2] = 0;	trans_[6] = 0;	trans_[10] = 1;	trans_[14] = 0;
	trans_[3] = 0;	trans_[7] = 0;	trans_[11] = 0;	trans_[15] = 1;
}

void AffineTransformation::multMatrix(double * m)
{
	double t[16];
	// Matrix Multiplication
	for (int k=0; k<16; k+=4)
	{
		for (int j=0; j<4; j++)
		{
			t[j+k] = 0;
			for (int i=0; i<4; i++)
				t[j+k] += m[4*i+j]*trans_[i+k];
		}
	}
	for (int k=0; k<16; k++)
	{
		trans_[k] = t[k];
	}
}

void AffineTransformation::translate(Vector t)
{
	double temp[16];
	temp[0] = 1;	temp[4] = 0;	temp[8] = 0;	temp[12] = t.x;
	temp[1] = 0;	temp[5] = 1;	temp[9] = 0;	temp[13] = t.y;
	temp[2] = 0;	temp[6] = 0;	temp[10] = 1;	temp[14] = t.z;
	temp[3] = 0;	temp[7] = 0;	temp[11] = 0;	temp[15] = 1;
	multMatrix(temp);
}

void AffineTransformation::rotate(Vector axis, double angle)
{
	// http://en.wikipedia.org/wiki/Rotation_matrix#Nested_dimensions
	double cA, sA;
	cA = cos(angle);
	sA = sin(angle);
	double temp[16];
	temp[0] = axis.x*axis.x*(1-cA)+cA;
	temp[1] = axis.x*axis.y*(1-cA)+axis.z*sA;
	temp[2] = axis.x*axis.z*(1-cA)-axis.y*sA;
	temp[3] = 0;
	temp[4] = axis.x*axis.y*(1-cA)-axis.z*sA;
	temp[5] = axis.y*axis.y*(1-cA)+cA;
	temp[6] = axis.y*axis.z*(1-cA)+axis.x*sA;
	temp[7] = 0;
	temp[8] = axis.x*axis.z*(1-cA)+axis.y*sA;
	temp[9] = axis.y*axis.z*(1-cA)-axis.x*sA;
	temp[10] = axis.z*axis.z*(1-cA)+cA;
	temp[11] = 0;
	temp[12] = 0; temp[13] = 0; temp[14] = 0; temp[15] = 1;
	multMatrix(temp);
}

void AffineTransformation::scale(Tool::Vec3f scale)
{
	double temp[16];
	temp[0] = scale.x;	temp[4] = 0;	temp[8] = 0;	temp[12] = 0;
	temp[1] = 0;	temp[5] = scale.y;	temp[9] = 0;	temp[13] = 0;
	temp[2] = 0;	temp[6] = 0;	temp[10] = scale.z;	temp[14] = 0;
	temp[3] = 0;	temp[7] = 0;	temp[11] = 0;	temp[15] = 1;
	multMatrix(temp);
}

// Drawing
void AffineTransformation::draw(GLWidget * dest) const
{
	if (shown())
	{
		Point p = position();
		glPushMatrix();
		glTranslatef(p.x, p.y, p.z);
		glMultMatrixd(trans_);
		Container::draw(dest);
		glPopMatrix();
	}
}

}
