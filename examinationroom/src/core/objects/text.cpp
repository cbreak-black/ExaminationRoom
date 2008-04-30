/*
 *  text.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 25.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "text.h"

#include "glwidget.h"

namespace Examination
{
	using namespace Tool;
	
Text::Text()
{
	font_ = QFont("Helvetica", 20);
	setName("Text");
}

// Serialisation
std::string Text::className() const
{
	return "Text";
}

std::string Text::toLua(std::ostream & outStream) const
{
	Object::toLua(outStream);
	outStream << name() << ":" << "setText(\"" << text().toAscii().data() << "\");\n";
	return name();
}

// Drawing
void Text::draw(GLWidget * dest) const
{
	if (shown())
	{
		Point p = position();
		// Load the correct color
		glColor4fv(color().vec);
		dest->renderText(p.x, p.y, p.z, text_, font_);
	}
}

QString Text::text() const
{
	return text_;
}

void Text::setText(QString t)
{
	text_ = t;
}

void Text::setText(const char * c)
{
	text_ = c;
}

}
