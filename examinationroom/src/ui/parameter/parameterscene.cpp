/*
 *  parameterscene.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 26.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterscene.h"

#include "scene.h"
#include "camera.h"

#include "parametercamera.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

namespace Examination
{

ParameterScene::ParameterScene(std::tr1::shared_ptr<Scene> scene)
	: ParameterObject(scene)
{
	// Create Object interface
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Scene"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Camera:")), 0, 0);
	QPushButton * button = new QPushButton("Camera...", this);
	g->addWidget(button, 0, 2, 1, 3);
	addWidget(b);

	connect(button, SIGNAL(clicked()), this, SLOT(showCameraDialog()));
}

void ParameterScene::showCameraDialog()
{
	if (scene())
		ParameterCamera::showCameraDialog(scene()->camera());
}

std::tr1::shared_ptr<Scene> ParameterScene::scene() const
{
	return std::tr1::dynamic_pointer_cast<Scene>(object());
}

}
