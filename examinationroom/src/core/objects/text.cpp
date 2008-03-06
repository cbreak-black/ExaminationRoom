/*
 *  text.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 25.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "text.h"

#include "vec.h"
#include "glwidget.h"

namespace Examination
{
	using namespace Tool;
	
Text::Text()
{
	font_ = QFont("Helvetica", 20);
}

// Drawing
void Text::draw(GLWidget * dest) const
{
	Point p = position();
	dest->renderText(p.x, p.y, p.z, text_, font_);
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
