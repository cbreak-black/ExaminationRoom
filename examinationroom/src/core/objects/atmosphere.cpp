/*
 *  atmosphere.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 02.04.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "atmosphere.h"

#include <qgl.h>

namespace Examination
{

Atmosphere::Atmosphere()
{
	setMode(Exp);
	setStart(0);
	setEnd(100);
	setDensity(1);
}

Atmosphere::FogMode Atmosphere::mode() const
{
	return fogMode_;
}

void Atmosphere::setMode(FogMode fogMode)
{
	fogMode_ = fogMode;
}

float Atmosphere::density() const
{
	return density_;
}

void Atmosphere::setDensity(float density)
{
	density_ = fabs(density);
}

float Atmosphere::start() const
{
	return start_;
}

void Atmosphere::setStart(float start)
{
	start_ = start;
}

float Atmosphere::end() const
{
	return end_;
}

void Atmosphere::setEnd(float end)
{
	end_ = end;
}

// Drawing
void Atmosphere::draw(GLWidget * dest) const
{
	if (shown())
	{
		// Enable fog
		glEnable(GL_FOG);
		// Set fog state
		switch (mode())
		{
			case  Exp:
				glFogi(GL_FOG_MODE, GL_EXP);
				glFogf(GL_FOG_DENSITY, density_);
				break;
			case  Exp2:
				glFogi(GL_FOG_MODE, GL_EXP2);
				glFogf(GL_FOG_DENSITY, density_);
				break;
			case  Linear:
				glFogi(GL_FOG_MODE, GL_LINEAR);
				glFogf(GL_FOG_START, start_);
				glFogf(GL_FOG_END, end_);
				break;
		}
		glFogfv(GL_FOG_COLOR, color().vec);
		// Draw the contents of this node
		Container::draw(dest);
		// Disable fog again
		glDisable(GL_FOG);
	}
}

}