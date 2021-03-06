/*
 *  affinetransformation.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 04.03.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "affinetransformation.h"

#include "parameter/parameteraffinetransformation.h"

#include <qgl.h>

#include "glerrortool.h"

#include "platform_math.h"

#include "luabridge.hpp"
#include "luahelper.h"

using namespace std::tr1;

namespace luabridge
{

template <>
struct tdstack <std::vector<double> >
{
private:
	static void push (lua_State *L, std::vector<double> data);
public:
	static std::vector<double> get (lua_State *L, int index)
	{
		luaL_checktype(L, index, LUA_TTABLE);
		std::vector<double> mat;
		for (int i = 1; i <= 16; i++)
		{
			lua_pushnumber(L, i);
			lua_gettable(L, index);
			mat.push_back(lua_tonumber(L, -1));
			lua_pop(L, 1);
		}
		return mat;
	}
};

}

namespace Examination
{
	using namespace Tool;
	
AffineTransformation::AffineTransformation()
{
	loadIdentity();
	setName("transformation");
}

ObjectPtr AffineTransformation::clone() const
{
	ContainerPtr c(new AffineTransformation(*this));
	c->clone(this);
	return c;
}

void AffineTransformation::loadIdentity()
{
	objectWillChange();
	trans_[0] = 1;	trans_[4] = 0;	trans_[8] = 0;	trans_[12] = 0;
	trans_[1] = 0;	trans_[5] = 1;	trans_[9] = 0;	trans_[13] = 0;
	trans_[2] = 0;	trans_[6] = 0;	trans_[10] = 1;	trans_[14] = 0;
	trans_[3] = 0;	trans_[7] = 0;	trans_[11] = 0;	trans_[15] = 1;
	objectDidChange();
}

void AffineTransformation::multMatrix(double * m)
{
	objectWillChange();
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
	objectDidChange();
}

void AffineTransformation::multMatrix(std::vector<double> m)
{
	if (m.size() < 16)
		return;
	objectWillChange();
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
	objectDidChange();
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

// Serialisation
const char * AffineTransformation::className_ = "AffineTransformation";

std::string AffineTransformation::className() const
{
	return AffineTransformation::className_;
}

std::string AffineTransformation::toLua(std::ostream & outStream) const
{
	Container::toLua(outStream);
	outStream << name() << ":" << "multMatrix({";
	for (int i = 0; i<15; i++)
		outStream << trans_[i] << ", ";
	outStream << trans_[15] << "});\n";
	return name();
}

// LUA
void AffineTransformation::registerLuaApi(luabridge::module * m)
{
	m->subclass<AffineTransformation,Container>(AffineTransformation::className_)
	.constructor<void (*)()>()
	.method("translate", &AffineTransformation::translate)
	.method("rotate", &AffineTransformation::rotate)
	.method("scale", &AffineTransformation::scale)
	.method<void (AffineTransformation::*)(std::vector<double>)>("multMatrix", &AffineTransformation::multMatrix);
}

std::tr1::shared_ptr<ParameterObject> AffineTransformation::createDialog()
{
	return std::tr1::shared_ptr<ParameterObject>(new ParameterAffineTransformation(sharedPtr()));
}

// Drawing
void AffineTransformation::draw(GLWidget * dest) const
{
	if (shown())
	{
		// If not enabled, just draw children and return
		if (!enabled())
		{
			Container::draw(dest);
		}
		else
		{
			Point p = position();
			glPushMatrix();
			glTranslatef(p.x, p.y, p.z);
			glMultMatrixd(trans_);
			GlErrorTool::getErrors("AffineTransformation::draw:1", name());
			Container::draw(dest);
			glPopMatrix();
			GlErrorTool::getErrors("AffineTransformation::draw:2", name());
		}
	}
}

}
