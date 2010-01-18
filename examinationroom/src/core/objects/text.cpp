/*
 *  text.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 25.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "text.h"

#include "parameter/parametertext.h"

#include "glwidget.h"

#include "glerrortool.h"

#include "luabridge.hpp"
#include "luahelper.h"

#include "camera.h"

#include <QGLFrameBufferObject>

namespace Examination
{
	using namespace Tool;

// The margin of the text boxes
const int textMargin = 8;
// The subsampling
const float pixelScale = 2;

Text::Text()
{
	renderedStringValid_ = false;
	renderedString_ = 0;
	renderedDimensions_ = Tool::Vec2f(0,0);
	font_ = QFont("Helvetica", 24*pixelScale);
	dimensions_ = Vec2f(256, 256);
	setName("text");
}

Text::Text(const Text & t)
	: Object(t)
{
	renderedStringValid_ = false;
	renderedString_ = NULL;
	font_ = t.font_;
	text_ = t.text_;
	dimensions_ = t.dimensions_;
}

Text::~Text()
{
	delete renderedString_;
}

ObjectPtr Text::clone() const
{
	return ObjectPtr(new Text(*this));
}

// Serialisation
const char * Text::className_ = "Text";

std::string Text::className() const
{
	return Text::className_;
}

std::string Text::toLua(std::ostream & outStream) const
{
	Object::toLua(outStream);
	outStream << name() << ":" << "setText(\"" << text().c_str() << "\");\n";
	outStream << name() << ":" << "setDimension({" << dimensions_.w << ", " << dimensions_.h << ");\n";
	return name();
}

// LUA
void Text::registerLuaApi(luabridge::module * m)
{
	m->subclass<Text,Object>(Text::className_)
	.constructor<void (*)()>()
	.method("text", &Text::text)
	.method<void (Text::*)(const char *)>("setText", &Text::setText)
	.method("dimensions", &Text::dimensions)
	.method("setDimensions", &Text::setDimensions);
}

// Drawing
void Text::draw(GLWidget *) const
{
	if (shown())
	{
		GlErrorTool::getErrors("Text::draw:1", name());
		if (!renderedStringValid_)
			updateRenderedString();
		drawRenderedString();
		GlErrorTool::getErrors("Text::draw:2", name());
	}
}

void Text::updateRenderedString() const
{
	GlErrorTool::getErrors("Text::updateRenderedString:1", name());
	// Get bounding box size
	Vec2f d = dimensions_*pixelScale;
	QFontMetrics fm(font_);
	QRect bb = fm.boundingRect(0, 0, d.w-textMargin*2, d.h-textMargin*2,
							   Qt::TextWordWrap, QString::fromStdString(text_));
	bb.adjust(-textMargin, -textMargin, textMargin, textMargin); // Add margin
	QSize bbs = bb.size();
	renderedDimensions_ = Vec2f(bbs.width(), bbs.height());
	// Dispose old FBO and create a new one
	if (renderedString_)
		delete renderedString_;
	renderedString_ = new QGLFramebufferObject(bbs, QGLFramebufferObject::CombinedDepthStencil);
	// Prepare for drawing
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// Draw into the FBO
	renderedString_->bind();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	QPainter p(renderedString_); // p.begin() implicit
	p.setFont(font_);
	p.setPen(QColor(Qt::white));
	// TODO: Transparency support
	p.setBackground(QColor(0,0,0,1));
	p.drawText(textMargin, textMargin, bbs.width()-textMargin, bbs.height()-textMargin,
			   Qt::TextWordWrap, QString::fromStdString(text_));
	p.end();
	renderedString_->release();
	// Cleanup from drawing
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// We're done
	renderedStringValid_ = true;
	GlErrorTool::getErrors("Text::updateRenderedString:2", name());
}

void Text::drawRenderedString() const
{
	if (renderedString_)
	{
		Point p = position();
		// Does not consider local transformations
		// Doubled to get original size
		float u = Camera::activeCamera()->unitScreenSize(p)*2;
		Vec2f s = renderedDimensions_/u;
		// Load the correct color
		glColor4fv(color().vec);
		glBindTexture(GL_TEXTURE_2D, renderedString_->texture());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(p.vec);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(p.x+s.w, p.y, p.z);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(p.x+s.w, p.y+s.h, p.z);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(p.x, p.y+s.h, p.z);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		GlErrorTool::getErrors("Text::drawRenderedString");
	}
}

std::string Text::text() const
{
	return text_;
}

void Text::setText(std::string t)
{
	objectWillChange();
	text_ = t;
	renderedStringValid_ = false;
	objectDidChange();
}

void Text::setText(const char * c)
{
	objectWillChange();
	text_ = c;
	renderedStringValid_ = false;
	objectDidChange();
}

Tool::Vec2f Text::dimensions() const
{
	return dimensions_;
}

void Text::setDimensions(const Tool::Vec2f & d)
{
	objectWillChange();
	dimensions_ = d;
	renderedStringValid_ = false;
	objectDidChange();
}

std::tr1::shared_ptr<ParameterObject> Text::createDialog()
{
	return std::tr1::shared_ptr<ParameterObject>(new ParameterText(sharedPtr()));
}

}
