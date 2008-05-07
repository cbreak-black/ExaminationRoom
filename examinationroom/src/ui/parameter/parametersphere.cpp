/*
 *  parametersphere.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 07.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parametersphere.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "objects/sphere.h"

namespace Examination
{

ParameterSphere::ParameterSphere(std::tr1::shared_ptr<Object> object) : ParameterObject(object)
{
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Sphere"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Radius:")), 0, 0);
	g->addWidget(new QLabel(tr("Slices:")), 1, 0);
	g->addWidget(new QLabel(tr("Stacks:")), 2, 0);
	lineRadius_ = new QLineEdit();
	lineSlices_ = new QLineEdit();
	lineStacks_ = new QLineEdit();
	lineRadius_->setAlignment(Qt::AlignRight);
	lineSlices_->setAlignment(Qt::AlignRight);
	lineStacks_->setAlignment(Qt::AlignRight);
	g->addWidget(lineRadius_, 0, 2, 1, 3);
	g->addWidget(lineSlices_, 1, 2, 1, 3);
	g->addWidget(lineStacks_, 2, 2, 1, 3);
	g->setRowStretch(3, 1);
	addWidget(b);

	connect(lineRadius_, SIGNAL(editingFinished()),
			this, SLOT(radiusEdited()));
	connect(lineSlices_, SIGNAL(editingFinished()),
			this, SLOT(slicesEdited()));
	connect(lineStacks_, SIGNAL(editingFinished()),
			this, SLOT(stacksEdited()));

	if (object)
	{
		reloadData();
	}
}

void ParameterSphere::reloadData()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Sphere> s = std::tr1::dynamic_pointer_cast<Sphere,Object>(o);
		if (s)
		{
			// Radius
			lineRadius_->setText(QString::number(s->radius(), 'g', 4));
			// Slices
			lineSlices_->setText(QString::number(s->slices()));
			// Stacks
			lineStacks_->setText(QString::number(s->stacks()));
		}
	}
}

void ParameterSphere::radiusEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Sphere> s = std::tr1::dynamic_pointer_cast<Sphere,Object>(o);
		if (s)
		{
			bool ok = false;
			float f = lineRadius_->text().toFloat(&ok);
			if (ok)
			{
				s->setRadius(f);
			}
		}
	}
}

void ParameterSphere::slicesEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Sphere> s = std::tr1::dynamic_pointer_cast<Sphere,Object>(o);
		if (s)
		{
			bool ok = false;
			int i = lineSlices_->text().toInt(&ok);
			if (ok)
			{
				s->setSlices(i);
			}
		}
	}
}

void ParameterSphere::stacksEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Sphere> s = std::tr1::dynamic_pointer_cast<Sphere,Object>(o);
		if (s)
		{
			bool ok = false;
			int i = lineStacks_->text().toInt(&ok);
			if (ok)
			{
				s->setStacks(i);
			}
		}
	}
}

}
