/*
 *  parametercamera.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 26.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parametercamera.h"

#include "camera.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

namespace Examination
{

ParameterCamera::ParameterCamera()
{
	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Camera"));
	b->setLayout(g);

	g->addWidget(new QLabel(tr("Type:")), 0, 0);
	g->addWidget(new QLabel(tr("Position:")), 1, 0);
	g->addWidget(new QLabel(tr("Direction:")), 2, 0);
	g->addWidget(new QLabel(tr("Up:")), 3, 0);
	g->addWidget(new QLabel(tr("Separation:")), 4, 0);
	g->addWidget(new QLabel(tr("Field of View:")), 5, 0);
	g->addWidget(new QLabel(tr("Paralax Plane:")), 6, 0);

	pulldownType_ = new QComboBox(this);
	pulldownType_->addItem("Perspective");
	pulldownType_->addItem("Parallel");
	pulldownType_->addItem("Screen");
	linePosX_ = new QLineEdit();
	linePosY_ = new QLineEdit();
	linePosZ_ = new QLineEdit();
	lineDirX_ = new QLineEdit();
	lineDirY_ = new QLineEdit();
	lineDirZ_ = new QLineEdit();
	lineUpX_ = new QLineEdit();
	lineUpY_ = new QLineEdit();
	lineUpZ_ = new QLineEdit();
	lineSep_ = new QLineEdit();
	lineFoV_ = new QLineEdit();
	linePP_ = new QLineEdit();

	g->addWidget(pulldownType_, 0, 2, 1, 3);
	g->addWidget(linePosX_, 1, 2);
	g->addWidget(linePosY_, 1, 3);
	g->addWidget(linePosZ_, 1, 4);
	g->addWidget(lineDirX_, 2, 2);
	g->addWidget(lineDirY_, 2, 3);
	g->addWidget(lineDirZ_, 2, 4);
	g->addWidget(lineUpX_, 3, 2);
	g->addWidget(lineUpY_, 3, 3);
	g->addWidget(lineUpZ_, 3, 4);
	g->addWidget(lineSep_, 4, 2, 1, 3);
	g->addWidget(lineFoV_, 5, 2, 1, 3);
	g->addWidget(linePP_, 6, 2, 1, 3);

	g->setRowStretch(7, 1);
	addWidget(b);

	connect(pulldownType_, SIGNAL(activated(int)),
			this, SLOT(typeActivated(int)));
	connect(linePosX_, SIGNAL(editingFinished()),
			this, SLOT(posEdited()));
	connect(linePosY_, SIGNAL(editingFinished()),
			this, SLOT(posEdited()));
	connect(linePosZ_, SIGNAL(editingFinished()),
			this, SLOT(posEdited()));
	connect(lineDirX_, SIGNAL(editingFinished()),
			this, SLOT(dirEdited()));
	connect(lineDirY_, SIGNAL(editingFinished()),
			this, SLOT(dirEdited()));
	connect(lineDirZ_, SIGNAL(editingFinished()),
			this, SLOT(dirEdited()));
	connect(lineUpX_, SIGNAL(editingFinished()),
			this, SLOT(upEdited()));
	connect(lineUpY_, SIGNAL(editingFinished()),
			this, SLOT(upEdited()));
	connect(lineUpZ_, SIGNAL(editingFinished()),
			this, SLOT(upEdited()));
	connect(lineSep_, SIGNAL(editingFinished()),
			this, SLOT(sepEdited()));
	connect(lineFoV_, SIGNAL(editingFinished()),
			this, SLOT(fovEdited()));
	connect(linePP_, SIGNAL(editingFinished()),
			this, SLOT(ppEdited()));
}

void ParameterCamera::loadCamera(std::tr1::shared_ptr<Camera> cam)
{
	cam_ = cam;

	Tool::Vec3f v;
	// Type
	pulldownType_->setCurrentIndex(cam_->type());
	// Position
	v = cam_->position();
	linePosX_->setText(QString::number(v.x, 'g', 3));
	linePosY_->setText(QString::number(v.y, 'g', 3));
	linePosZ_->setText(QString::number(v.z, 'g', 3));
	// Direction
	v = cam_->direction();
	lineDirX_->setText(QString::number(v.x, 'g', 3));
	lineDirY_->setText(QString::number(v.y, 'g', 3));
	lineDirZ_->setText(QString::number(v.z, 'g', 3));
	// Up
	v = cam_->up();
	lineUpX_->setText(QString::number(v.x, 'g', 3));
	lineUpY_->setText(QString::number(v.y, 'g', 3));
	lineUpZ_->setText(QString::number(v.z, 'g', 3));
	// Sep
	lineSep_->setText(QString::number(cam_->separation(), 'g', 3));
	lineFoV_->setText(QString::number(cam_->fieldOfView(), 'g', 3));
	linePP_->setText(QString::number(cam_->paralaxPlane(), 'g', 3));
}

void ParameterCamera::typeActivated(int i)
{
	if (cam_)
		cam_->setType((Camera::Type)i);
}

void ParameterCamera::posEdited()
{
	if (cam_)
	{
		bool okX, okY, okZ;
		float fX = linePosX_->text().toFloat(&okX);
		float fY = linePosY_->text().toFloat(&okY);
		float fZ = linePosZ_->text().toFloat(&okZ);
		if (okX && okY && okZ)
		{
			cam_->setPosition(Tool::Vec3f(fX, fY, fZ));
		}
	}
}

void ParameterCamera::dirEdited()
{
	if (cam_)
	{
		bool okX, okY, okZ;
		float fX = lineDirX_->text().toFloat(&okX);
		float fY = lineDirY_->text().toFloat(&okY);
		float fZ = lineDirZ_->text().toFloat(&okZ);
		if (okX && okY && okZ)
		{
			cam_->setDirection(Tool::Vec3f(fX, fY, fZ));
		}
	}
}

void ParameterCamera::upEdited()
{
	if (cam_)
	{
		bool okX, okY, okZ;
		float fX = lineUpX_->text().toFloat(&okX);
		float fY = lineUpY_->text().toFloat(&okY);
		float fZ = lineUpZ_->text().toFloat(&okZ);
		if (okX && okY && okZ)
		{
			cam_->setUp(Tool::Vec3f(fX, fY, fZ));
		}
	}
}

void ParameterCamera::sepEdited()
{
	if (cam_)
	{
		bool ok;
		float f = lineSep_->text().toFloat(&ok);
		if (ok)
		{
			cam_->setSeparation(f);
		}
	}
}

void ParameterCamera::fovEdited()
{
	if (cam_)
	{
		bool ok;
		float f = lineFoV_->text().toFloat(&ok);
		if (ok)
		{
			cam_->setFieldOfView(f);
		}
	}
}

void ParameterCamera::ppEdited()
{
	if (cam_)
	{
		bool ok;
		float f = linePP_->text().toFloat(&ok);
		if (ok)
		{
			cam_->setParalaxPlane(f);
		}
	}
}

void ParameterCamera::closeEvent(QCloseEvent * /* event */)
{
	cam_.reset();
	// If this dialog is deleted too late, program will crash
	// due to error with QComboBox:
	// Object::disconnect: No such signal QObject::rowsAboutToBeRemoved(QModelIndex,int,int)
	cameraDialog_.reset();
}

std::tr1::shared_ptr<ParameterCamera> ParameterCamera::cameraDialog_;

void ParameterCamera::showCameraDialog(std::tr1::shared_ptr<Camera> cam)
{
	if (!cameraDialog_)
	{
		cameraDialog_ = std::tr1::shared_ptr<ParameterCamera>(new ParameterCamera);
	}
	cameraDialog_->loadCamera(cam);
	cameraDialog_->show();
}

}
