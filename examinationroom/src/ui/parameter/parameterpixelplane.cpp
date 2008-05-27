/*
 *  parameterpixelplane.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 07.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parameterpixelplane.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

#include "objects/pixelplane.h"

namespace Examination
{

ParameterPixelplane::ParameterPixelplane(std::tr1::shared_ptr<Object> object) : ParameterObject(object)
{
	// Create Object interface
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Pixelplane"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Size:")), 0, 0);
	g->addWidget(new QLabel(tr("Zoom:")), 1, 0);
	lineSizeX_ = new QLineEdit();
	lineSizeY_ = new QLineEdit();
	lineZoomX_ = new QLineEdit();
	lineZoomY_ = new QLineEdit();
	lineSizeX_->setAlignment(Qt::AlignRight);
	lineSizeY_->setAlignment(Qt::AlignRight);
	lineZoomX_->setAlignment(Qt::AlignRight);
	lineZoomY_->setAlignment(Qt::AlignRight);
	checkboxAutoscale_ = new QCheckBox("Autoscale", this);
	g->addWidget(lineSizeX_, 0, 2);
	g->addWidget(lineSizeY_, 0, 3);
	g->addWidget(lineZoomX_, 1, 2);
	g->addWidget(lineZoomY_, 1, 3);
	g->addWidget(checkboxAutoscale_, 2, 0, 1, 4);
	g->setRowStretch(3, 1);
	addWidget(b);

	connect(lineSizeX_, SIGNAL(editingFinished()),
			this, SLOT(sizeEdited()));
	connect(lineSizeY_, SIGNAL(editingFinished()),
			this, SLOT(sizeEdited()));
	connect(lineZoomX_, SIGNAL(editingFinished()),
			this, SLOT(zoomEdited()));
	connect(lineZoomY_, SIGNAL(editingFinished()),
			this, SLOT(zoomEdited()));
	connect(checkboxAutoscale_, SIGNAL(stateChanged(int)),
			this, SLOT(autoscaleStateChanged(int)));

	if (object)
	{
		reloadData();
	}
}

void ParameterPixelplane::reloadData()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Pixelplane> p = std::tr1::dynamic_pointer_cast<Pixelplane,Object>(o);
		if (p)
		{
			// Size
			Tool::Vec2f size = p->size();
			lineSizeX_->setText(QString::number(size.x, 'g', 3));
			lineSizeY_->setText(QString::number(size.y, 'g', 3));
			// Zoom
			Tool::Vec2f zoom = p->zoom();
			lineZoomX_->setText(QString::number(zoom.x, 'g', 3));
			lineZoomY_->setText(QString::number(zoom.y, 'g', 3));
			// Wireframe
			checkboxAutoscale_->setCheckState(p->autoResize()? Qt::Checked : Qt::Unchecked);
		}
	}
}

// Slots from the UI
void ParameterPixelplane::sizeEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Pixelplane> p = std::tr1::dynamic_pointer_cast<Pixelplane,Object>(o);
		if (p)
		{
			bool okX, okY;
			float fx = lineSizeX_->text().toFloat(&okX);
			float fy = lineSizeY_->text().toFloat(&okY);
			if (okX && okY)
			{
				p->setSize(fx, fy);
			}
			else
			{
				reloadData();
			}
		}
	}
}

void ParameterPixelplane::zoomEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Pixelplane> p = std::tr1::dynamic_pointer_cast<Pixelplane,Object>(o);
		if (p)
		{
			bool okX, okY;
			float fx = lineZoomX_->text().toFloat(&okX);
			float fy = lineZoomY_->text().toFloat(&okY);
			if (okX && okY)
			{
				p->setZoom(fx, fy);
			}
			else
			{
				reloadData();
			}
		}
	}
}

void ParameterPixelplane::autoscaleStateChanged(int state)
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Pixelplane> p = std::tr1::dynamic_pointer_cast<Pixelplane,Object>(o);
		if (p)
		{
			p->setAutoResize(state == Qt::Checked);
		}
	}
}

}
