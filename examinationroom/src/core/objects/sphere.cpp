/*
 *  sphere.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 02.04.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "sphere.h"

#include "surfaces/abstracttexture.h"

#include <qgl.h>

namespace Examination
{

Sphere::Sphere()
{
	radius_ = 1;
	slices_ = 24;
	stacks_ = 12;
	setName("Sphere");
}

// Drawing
void Sphere::draw(GLWidget * dest) const
{
	if (shown())
	{
		GLUquadricObj * quad = gluNewQuadric();
		if (texture())
		{
			texture()->glBindTex(dest);
			gluQuadricTexture(quad,GL_TRUE);
		}
		glColor4fv(color().vec);
		if (wireframe())
		{
			gluQuadricDrawStyle(quad, GLU_LINE);
		}
		else
		{
			gluQuadricDrawStyle(quad, GLU_FILL);
		}
		Tool::Vector v = position();
		glPushMatrix();
		glTranslatef(v.x, v.y, v.z);
		gluSphere(quad, radius_, slices_, stacks_);
		glPopMatrix();
		if (texture())
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		gluDeleteQuadric(quad);
	}
}

float Sphere::radius() const
{
	return radius_;
}

void Sphere::setRadius(float radius)
{
	radius_ = radius;
}

int Sphere::slices() const
{
	return slices_;
}

void Sphere::setSlices(int slices)
{
	slices_ = slices;
}

int Sphere::stacks() const
{
	return stacks_;
}

void Sphere::setStacks(int stacks)
{
	stacks_ = stacks;
}

// Serialisation
std::string Sphere::className() const
{
	return "Sphere";
}

std::string Sphere::toLua(std::ostream & outStream) const
{
	Object::toLua(outStream);
	outStream << name() << ":" << "setRadius(" << radius() << ");\n";
	outStream << name() << ":" << "setSlices(" << slices() << ");\n";
	outStream << name() << ":" << "setStacks(" << stacks() << ");\n";
	return name();
}


}
