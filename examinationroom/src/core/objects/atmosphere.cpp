/*
 *  atmosphere.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 02.04.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "atmosphere.h"

#include "parameter/parameteratmosphere.h"

#include <qgl.h>

namespace Examination
{

Atmosphere::Atmosphere()
{
	setMode(Exp);
	setStart(0);
	setEnd(100);
	setDensity(1);
	setName("Atmosphere");
}

Atmosphere::FogMode Atmosphere::mode() const
{
	return fogMode_;
}

void Atmosphere::setMode(FogMode fogMode)
{
	objectWillChange();
	fogMode_ = fogMode;
	objectDidChange();
}

float Atmosphere::density() const
{
	return density_;
}

void Atmosphere::setDensity(float density)
{
	objectWillChange();
	density_ = fabs(density);
	objectDidChange();
}

float Atmosphere::start() const
{
	return start_;
}

void Atmosphere::setStart(float start)
{
	objectWillChange();
	start_ = start;
	objectDidChange();
}

float Atmosphere::end() const
{
	return end_;
}

void Atmosphere::setEnd(float end)
{
	objectWillChange();
	end_ = end;
	objectDidChange();
}

// Serialisation
std::string Atmosphere::className() const
{
	return "Atmosphere";
}

std::string Atmosphere::toLua(std::ostream & outStream) const
{
	Container::toLua(outStream);
	outStream << name() << ":" << "setMode(\"";
	switch (mode())
	{
		case Atmosphere::Exp:
			outStream << "Exp";
			break;
		case Atmosphere::Exp2:
			outStream << "Exp2";
			break;
		case Atmosphere::Linear:
			outStream << "Linear";
			break;
	}
	outStream << "\");\n";
	outStream << name() << ":" << "setDensity(" << density() << ");\n";
	outStream << name() << ":" << "setStart(" << start() << ");\n";
	outStream << name() << ":" << "setEnd(" << end() << ");\n";
	return name();
}

std::tr1::shared_ptr<ParameterObject> Atmosphere::createDialog()
{
	return std::tr1::shared_ptr<ParameterObject>(new ParameterAtmosphere(sharedPtr()));
}

// Drawing
void Atmosphere::draw(GLWidget * dest) const
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

}
