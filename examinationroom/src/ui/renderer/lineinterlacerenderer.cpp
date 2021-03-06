/*
 *  lineinterlacerenderer.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "lineinterlacerenderer.h"

#include "scene.h"
#include "camera.h"
#include "glwidget.h"

#include "glerrortool.h"

namespace Examination
{

LineInterlaceRenderer::LineInterlaceRenderer(std::tr1::shared_ptr<Scene> scene)
	: AbstractRenderer(scene)
{
}

void LineInterlaceRenderer::renderScene(GLWidget * w)
{
	lineStencil();

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0x01, 0x01);

	w->setSide(GLWidget::left);
	scene()->draw(w);

	glClear(GL_DEPTH_BUFFER_BIT);
	glStencilFunc(GL_EQUAL, 0x01, 0x01);

	w->setSide(GLWidget::right);
	scene()->draw(w);

	glDisable(GL_STENCIL_TEST);

	GlErrorTool::getErrors("LineInterlaceRenderer::renderScene");
}

static const GLubyte stipplePattern[] = {
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff
};

void LineInterlaceRenderer::lineStencil()
{
	// Prepare projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// Prepare stencil
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 0x01, 0x01);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
	// Lock buffers
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glDepthMask(GL_FALSE);
	// Draw The pattern into the stencil buffer
	glColor4f(1,1,1,1);
	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple(stipplePattern);
	glRectf(0,0,1,1);
	glDisable(GL_POLYGON_STIPPLE);
	// Unlock buffers
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDepthMask(GL_TRUE);
	// Disable stencil
	glStencilFunc(GL_EQUAL, 0x01, 0x01);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glDisable(GL_STENCIL_TEST);
	// Restore projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

}
