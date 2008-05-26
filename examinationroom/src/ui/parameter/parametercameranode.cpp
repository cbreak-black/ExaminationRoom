/*
 *  parametercameranode.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 26.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parametercameranode.h"

#include "objects/cameranode.h"
#include "camera.h"

#include "parametercamera.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

namespace Examination
{

ParameterCameraNode::ParameterCameraNode(std::tr1::shared_ptr<Object> object)
	: ParameterObject(object)
{
	// Create Object interface
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("CameraNode"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Camera:")), 0, 0);
	QPushButton * button = new QPushButton("Camera...", this);
	g->addWidget(button, 0, 2, 1, 3);
	addWidget(b);

	connect(button, SIGNAL(clicked()), this, SLOT(showCameraDialog()));
}

void ParameterCameraNode::showCameraDialog()
{
	std::tr1::shared_ptr<CameraNode> camNode = std::tr1::dynamic_pointer_cast<CameraNode>(object());
	if (camNode)
		ParameterCamera::showCameraDialog(camNode->camera());
}

}
