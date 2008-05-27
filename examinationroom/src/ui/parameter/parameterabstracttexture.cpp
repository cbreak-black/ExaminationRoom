/*
 *  parameterabstracttexture.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 26.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterabstracttexture.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "vec.h"

#include "surfaces/abstracttexture.h"

namespace Examination
{

ParameterAbstractTexture::ParameterAbstractTexture(std::tr1::shared_ptr<AbstractTexture> texture)
{
	texture_ = texture;

	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Texture"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Size:")), 0, 0);
	g->addWidget(new QLabel(tr("Zoom:")), 1, 0);
	lineSizeX_ = new QLineEdit();
	lineSizeY_ = new QLineEdit();
	QPushButton * sizeReset = new QPushButton("R", this);
	sizeReset->resize(26, 26);
	lineZoomX_ = new QLineEdit();
	lineZoomY_ = new QLineEdit();
	lineSizeX_->setAlignment(Qt::AlignRight);
	lineSizeY_->setAlignment(Qt::AlignRight);
	lineZoomX_->setAlignment(Qt::AlignRight);
	lineZoomY_->setAlignment(Qt::AlignRight);
	g->addWidget(lineSizeX_, 0, 2);
	g->addWidget(lineSizeY_, 0, 3);
	g->addWidget(sizeReset, 0, 4);
	g->addWidget(lineZoomX_, 1, 2);
	g->addWidget(lineZoomY_, 1, 3);
	g->setRowStretch(2, 1);

	connect(lineSizeX_, SIGNAL(editingFinished()),
			this, SLOT(sizeEdited()));
	connect(lineSizeY_, SIGNAL(editingFinished()),
			this, SLOT(sizeEdited()));
	connect(sizeReset, SIGNAL(clicked()),
			this, SLOT(sizeReset()));
	connect(lineZoomX_, SIGNAL(editingFinished()),
			this, SLOT(zoomEdited()));
	connect(lineZoomY_, SIGNAL(editingFinished()),
			this, SLOT(zoomEdited()));

	addWidget(b);

	reloadData();
}

void ParameterAbstractTexture::reloadData()
{
	AbstractTexturePtr tex = texture();
	if (tex)
	{
		// Size
		Tool::Vec2f size(tex->width(), tex->height());
		lineSizeX_->setText(QString::number((int)size.x));
		lineSizeY_->setText(QString::number((int)size.y));
		// Zoom
		Tool::Vec2f zoom = tex->zoom();
		lineZoomX_->setText(QString::number(zoom.x, 'g', 3));
		lineZoomY_->setText(QString::number(zoom.y, 'g', 3));
	}
}

// Slots from the UI
void ParameterAbstractTexture::sizeEdited()
{
	if (texture())
	{
		bool okX, okY;
		float fx = lineSizeX_->text().toFloat(&okX);
		float fy = lineSizeY_->text().toFloat(&okY);
		if (okX && okY)
		{
			texture()->resizeTo(fx, fy);
		}
		else
		{
			reloadData();
		}
	}
}

void ParameterAbstractTexture::sizeReset()
{
	if (texture())
	{
		texture()->resizeToOriginal();
		reloadData();
	}
}

void ParameterAbstractTexture::zoomEdited()
{
	if (texture())
	{
		bool okX, okY;
		float fx = lineZoomX_->text().toFloat(&okX);
		float fy = lineZoomY_->text().toFloat(&okY);
		if (okX && okY)
		{
			texture()->setZoom(fx, fy);
		}
		else
		{
			reloadData();
		}
	}
}

std::tr1::shared_ptr<AbstractTexture> ParameterAbstractTexture::texture() const
{
	return texture_.lock();
}

}
