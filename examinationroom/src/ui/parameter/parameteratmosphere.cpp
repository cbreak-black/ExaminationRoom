/*
 *  parameteratmosphere.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 08.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parameteratmosphere.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

#include "objects/atmosphere.h"

namespace Examination
{

ParameterAtmosphere::ParameterAtmosphere(std::tr1::shared_ptr<Object> object)
	: ParameterObject(object)
{
	// Create Object interface
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Atmosphere"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Density:")), 0, 0);
	g->addWidget(new QLabel(tr("Start:")), 1, 0);
	g->addWidget(new QLabel(tr("End:")), 2, 0);
	g->addWidget(new QLabel(tr("Mode:")), 3, 0);
	lineDensity_ = new QLineEdit(this);
	lineStart_ = new QLineEdit(this);
	lineEnd_ = new QLineEdit(this);
	lineDensity_->setAlignment(Qt::AlignRight);
	lineStart_->setAlignment(Qt::AlignRight);
	lineEnd_->setAlignment(Qt::AlignRight);
	pulldownMode_ = new QComboBox(this);
	pulldownMode_->addItem("Exp");
	pulldownMode_->addItem("Exp2");
	pulldownMode_->addItem("Linear");
	g->addWidget(lineDensity_, 0, 2, 1, 3);
	g->addWidget(lineStart_, 1, 2, 1, 3);
	g->addWidget(lineEnd_, 2, 2, 1, 3);
	g->addWidget(pulldownMode_, 3, 2, 1, 3);
	g->setRowStretch(4, 1);
	addWidget(b);

	connect(lineDensity_, SIGNAL(editingFinished()),
			this, SLOT(densityEdited()));
	connect(lineStart_, SIGNAL(editingFinished()),
			this, SLOT(startEdited()));
	connect(lineEnd_, SIGNAL(editingFinished()),
			this, SLOT(endEdited()));
	connect(pulldownMode_, SIGNAL(activated(int)),
			this, SLOT(modeActivated(int)));

	if (object)
	{
		reloadData();
	}
}

void ParameterAtmosphere::reloadData()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Atmosphere> a = std::tr1::dynamic_pointer_cast<Atmosphere,Object>(o);
		if (a)
		{
			// Density
			lineDensity_->setText(QString::number(a->density(), 'g', 4));
			// Start
			lineStart_->setText(QString::number(a->start(), 'g', 4));
			// End
			lineEnd_->setText(QString::number(a->end(), 'g', 4));
			// Mode
			pulldownMode_->setCurrentIndex(a->mode());
		}
	}
}

// Slots from the UI
void ParameterAtmosphere::densityEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Atmosphere> a = std::tr1::dynamic_pointer_cast<Atmosphere,Object>(o);
		if (a)
		{
			bool ok;
			float f = lineDensity_->text().toFloat(&ok);
			if (ok)
			{
				a->setDensity(f);
			}
		}
	}
}

void ParameterAtmosphere::startEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Atmosphere> a = std::tr1::dynamic_pointer_cast<Atmosphere,Object>(o);
		if (a)
		{
			bool ok;
			float f = lineStart_->text().toFloat(&ok);
			if (ok)
			{
				a->setStart(f);
			}
		}
	}
}

void ParameterAtmosphere::endEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Atmosphere> a = std::tr1::dynamic_pointer_cast<Atmosphere,Object>(o);
		if (a)
		{
			bool ok;
			float f = lineEnd_->text().toFloat(&ok);
			if (ok)
			{
				a->setEnd(f);
			}
		}
	}
}

void ParameterAtmosphere::modeActivated(int mode)
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Atmosphere> a = std::tr1::dynamic_pointer_cast<Atmosphere,Object>(o);
		if (a)
		{
			if (mode < 3 && mode >= 0)
			{
				a->setMode((Atmosphere::FogMode)mode);
			}
		}
	}
}

}
