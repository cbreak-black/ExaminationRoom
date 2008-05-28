/*
 *  parameterstereogram.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 27.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterstereogram.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

#include "surfaces/stereogram.h"
#include "surfaces/texture.h"

namespace Examination
{

ParameterStereogram::ParameterStereogram(std::tr1::shared_ptr<AbstractTexture> texture)
	: ParameterAbstractTexture(texture)
{
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Stereogram"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Depth:")), 0, 0);
	g->addWidget(new QLabel(tr("Left:")), 1, 0);
	g->addWidget(new QLabel(tr("Right:")), 2, 0);
	g->addWidget(new QLabel(tr("Offset:")), 3, 0);
	linePathDepth_ = new QLineEdit();
	linePathLeft_ = new QLineEdit();
	linePathRight_ = new QLineEdit();
	lineOffset_ = new QLineEdit();
	lineOffset_->setAlignment(Qt::AlignRight);
	comboStyle_ = new QComboBox(this);
	comboStyle_->addItem("Convex");
	comboStyle_->addItem("Concave");
	g->addWidget(linePathDepth_, 0, 2, 1, 3);
	g->addWidget(linePathLeft_, 1, 2, 1, 3);
	g->addWidget(linePathRight_, 2, 2, 1, 3);
	g->addWidget(lineOffset_, 3, 2, 1, 3);
	g->addWidget(comboStyle_, 4, 2, 1, 3);
	g->setRowStretch(5, 1);

	addWidget(b);

	connect(linePathDepth_, SIGNAL(editingFinished()),
			this, SLOT(pathDepthEdited()));
	connect(linePathLeft_, SIGNAL(editingFinished()),
			this, SLOT(pathLeftEdited()));
	connect(linePathRight_, SIGNAL(editingFinished()),
			this, SLOT(pathRightEdited()));
	connect(lineOffset_, SIGNAL(editingFinished()),
			this, SLOT(offsetEdited()));
	connect(comboStyle_, SIGNAL(activated(int)),
			this, SLOT(styleActivated(int)));

	reloadData();
}

void ParameterStereogram::reloadData()
{
	ParameterAbstractTexture::reloadData();
	if (texture())
	{
		std::tr1::shared_ptr<Stereogram> tex = std::tr1::dynamic_pointer_cast<Stereogram>(texture());
		if (tex)
		{
			// Paths
			if (tex->texDepth())
				linePathDepth_->setText(QString::fromStdString(tex->texDepth()->path()));
			if (tex->texLeft())
				linePathLeft_->setText(QString::fromStdString(tex->texLeft()->path()));
			if (tex->texRight())
				linePathRight_->setText(QString::fromStdString(tex->texRight()->path()));
			lineOffset_->setText(QString::number(tex->offset()));
			comboStyle_->setCurrentIndex(tex->style());
		}
	}
}

void ParameterStereogram::pathDepthEdited()
{
	if (texture())
	{
		std::tr1::shared_ptr<Stereogram> tex = std::tr1::dynamic_pointer_cast<Stereogram>(texture());
		if (tex && tex->texDepth())
		{
			// Path
			tex->texDepth()->setPath(linePathDepth_->text().toStdString());
			tex->recreateStereogram();
			reloadData(); // No auto reload, unlike for objects
		}
	}
}

void ParameterStereogram::pathLeftEdited()
{
	if (texture())
	{
		std::tr1::shared_ptr<Stereogram> tex = std::tr1::dynamic_pointer_cast<Stereogram>(texture());
		if (tex && tex->texLeft())
		{
			// Path
			tex->texLeft()->setPath(linePathLeft_->text().toStdString());
			reloadData(); // No auto reload, unlike for objects
		}
	}
}

void ParameterStereogram::pathRightEdited()
{
	if (texture())
	{
		std::tr1::shared_ptr<Stereogram> tex = std::tr1::dynamic_pointer_cast<Stereogram>(texture());
		if (tex && tex->texRight())
		{
			// Path
			tex->texRight()->setPath(linePathRight_->text().toStdString());
			reloadData(); // No auto reload, unlike for objects
		}
	}
}

void ParameterStereogram::offsetEdited()
{
	if (texture())
	{
		std::tr1::shared_ptr<Stereogram> tex = std::tr1::dynamic_pointer_cast<Stereogram>(texture());
		if (tex)
		{
			// Offset
			bool ok;
			int i = lineOffset_->text().toInt(&ok);
			if (ok)
			{
				tex->setOffset(i);
			}
			reloadData(); // No auto reload, unlike for objects
		}
	}
}

void ParameterStereogram::styleActivated(int i)
{
	if (texture())
	{
		std::tr1::shared_ptr<Stereogram> tex = std::tr1::dynamic_pointer_cast<Stereogram>(texture());
		if (tex)
		{
			tex->setStyle((Stereogram::Style)i);
		}
	}
}

}
