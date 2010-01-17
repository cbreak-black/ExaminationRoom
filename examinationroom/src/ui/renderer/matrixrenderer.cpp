/*
 *  matrixrenderer.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "matrixrenderer.h"

#include "scene.h"
#include "camera.h"
#include "glwidget.h"

#include "glerrortool.h"

namespace Examination
{

static const float colorMatrixLeft[16] = {
	1, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0.5, 0,
	0, 0, 0, 1
};
static const float colorMatrixRight[16] = {
	0, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 0.5, 0,
	0, 0, 0, 1
};

MatrixRenderer::MatrixRenderer(std::tr1::shared_ptr<Scene> scene)
	: AbstractRenderer(scene)
{
}

void MatrixRenderer::renderScene(GLWidget * w)
{
	QSize s = w->size();
	glDrawBuffer(GL_AUX1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawBuffer(GL_AUX0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw left
	w->setSide(GLWidget::left);
	scene()->draw(w);

	// Prepare for drawing into the right aux buffer
	glDrawBuffer(GL_AUX1);
	// Draw right
	w->setSide(GLWidget::right);
	scene()->draw(w);

	// Write composite into the back buffer
	glDrawBuffer(GL_BACK);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// From left aux buffer with left color transform
	glReadBuffer(GL_AUX0);
	glMatrixMode(GL_COLOR);
	glLoadMatrixf(colorMatrixLeft);
	glMatrixMode(GL_MODELVIEW);
	glCopyPixels(0,0,s.width(),s.height(),GL_COLOR);
	// From right aux buffer with right color transform
	glReadBuffer(GL_AUX1);
	glMatrixMode(GL_COLOR);
	glLoadMatrixf(colorMatrixRight);
	glMatrixMode(GL_MODELVIEW);
	glCopyPixels(0,0,s.width(),s.height(),GL_COLOR);
	// Restore color matrix
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_COLOR);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	GlErrorTool::getErrors("MatrixRenderer::renderScene");
}

}
