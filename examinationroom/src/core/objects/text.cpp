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
#include "screenproject.h"

#include <QPainter>
#include <QGLFrameBufferObject>

// Marshaling
namespace luabridge
{
	const char * textTypes[] =
	{
		"Label",
		"Plane",
		0
	};
	template <>
	struct tdstack <Examination::Text::Type>
	{
		static void push (lua_State *L, Examination::Text::Type data)
		{
			lua_pushstring(L, textTypes[data]);
		}
		static Examination::Text::Type get (lua_State *L, int index)
		{
			return static_cast<Examination::Text::Type>(luaL_checkoption(L, index, 0, textTypes));
		}
	};
}

namespace Examination
{
	using namespace Tool;

// The margin of the text boxes
const int textMargin = 8;
// The subsampling
const float pixelScale = 1;

// Tex Coordinate array
const Vec2f texCoords[] = {
	Vec2f(0.0f,0.0f),
	Vec2f(1.0f,0.0f),
	Vec2f(1.0f,1.0f),
	Vec2f(0.0f,1.0f)
};

Text::Text()
{
	renderedStringValid_ = false;
	renderedString_ = NULL;
	renderedDimensions_ = Tool::Vec2f(0,0);
	font_ = QFont("Helvetica", 24*pixelScale);
	dimensions_ = Vec2f(256, 256);
	type_ = Label;
	setName("text");
}

Text::Text(const Text & t)
	: Object(t)
{
	renderedStringValid_ = false;
	renderedString_ = NULL;
	renderedDimensions_ = Tool::Vec2f(0,0);
	font_ = t.font_;
	text_ = t.text_;
	type_ = t.type_;
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
	outStream << name() << ":" << "setType(\"";
	switch (type_)
	{
		case Text::Label:
			outStream << "Label";
			break;
		case Text::Plane:
			outStream << "Plane";
			break;
	}
	outStream << "\");\n";
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
	.method("type", &Text::type)
	.method("setType", &Text::setType)
	.method("dimensions", &Text::dimensions)
	.method("setDimensions", &Text::setDimensions);
}

// Drawing
void Text::draw(GLWidget *) const
{
	if (shown())
	{
		GlErrorTool::getErrors("Text::draw:1", name());
		switch(type())
		{
			case Text::Label:
				drawRenderedStringLabel();
				break;
			case Text::Plane:
				drawRenderedStringPlane();
				break;
		};
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

void Text::drawRenderedStringLabel() const
{
	if (renderedString_)
	{
		ScreenProject sp;
		sp.calculateMVP();
		Point rect[4], proj[4]; // Rectangle vertex, projected vertex
		Vec2f s = renderedDimensions_/pixelScale;
		// Transform origin to screen space, calculate other three corners
		// then transform back to world space for drawing
		rect[0] = position();
		proj[0] = sp.transformToScreenSpace(rect[0]);
		proj[1] = Point(proj[0].x+s.w, proj[0].y, proj[0].z);
		proj[2] = Point(proj[0].x+s.w, proj[0].y+s.h, proj[0].z);
		proj[3] = Point(proj[0].x, proj[0].y+s.h, proj[0].z);
		rect[1] = sp.transformToWorldSpace(proj[1]);
		rect[2] = sp.transformToWorldSpace(proj[2]);
		rect[3] = sp.transformToWorldSpace(proj[3]);
		drawRenderedString(rect);
		GlErrorTool::getErrors("Text::drawRenderedStringLabel");
	}
}

void Text::drawRenderedStringPlane() const
{
	if (renderedString_)
	{
		Point rect[4];
		rect[0] = position();
		float u = Camera::activeCamera()->unitScreenSize(rect[0])*pixelScale;
		Vec2f s = renderedDimensions_/u;
		rect[1] = Point(rect[0].x+s.w, rect[0].y, rect[0].z);
		rect[2] = Point(rect[0].x+s.w, rect[0].y+s.h, rect[0].z);
		rect[3] = Point(rect[0].x, rect[0].y+s.h, rect[0].z);
		drawRenderedString(rect);
		GlErrorTool::getErrors("Text::drawRenderedStringPlane");
	}
}

// Callers must ensure that rect is a valid 4 element array of points
// as well as the existence and validity of renderedString_
void Text::drawRenderedString(Vec3f * rect) const
{
	// Color
	glColor4fv(color().vec);
	// Normal
	Vector n = cross(rect[1]-rect[0], rect[2]-rect[0]);
	n.normalize();
	glNormal3fv(n.vec);
	// Draw the rectangle
	glBindTexture(GL_TEXTURE_2D, renderedString_->texture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, rect);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::string Text::text() const
{
	return text_;
}

void Text::setText(std::string t)
{
	objectWillChange();
	text_ = t;
	updateRenderedString();
	objectDidChange();
}

void Text::setText(const char * c)
{
	objectWillChange();
	text_ = c;
	updateRenderedString();
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
	updateRenderedString();
	objectDidChange();
}

Text::Type Text::type() const
{
	return type_;
}

void Text::setType(Text::Type t)
{
	objectWillChange();
	type_ = t;
	objectDidChange();
}

std::tr1::shared_ptr<ParameterObject> Text::createDialog()
{
	return std::tr1::shared_ptr<ParameterObject>(new ParameterText(sharedPtr()));
}

}
