/*
 *  parameterobject.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 04.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterobject.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "objects/object.h"

namespace Examination
{

ParameterObject::ParameterObject(std::tr1::shared_ptr<Object> object)
{
	object_ = object;

	// Create Object interface
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Object"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Name:")), 0, 0);
	g->addWidget(new QLabel(tr("Position:")), 1, 0);
	g->addWidget(new QLabel(tr("Color:")), 2, 0);
	lineName_ = new QLineEdit();
	linePosX_ = new QLineEdit();
	linePosY_ = new QLineEdit();
	linePosZ_ = new QLineEdit();
	colorLabel_ = new QLabel();
	colorLabel_->setText("color");
	colorLabel_->setAutoFillBackground(true);
	g->addWidget(lineName_, 0, 2, 1, 3);
	g->addWidget(linePosX_, 1, 2);
	g->addWidget(linePosY_, 1, 3);
	g->addWidget(linePosZ_, 1, 4);
	g->addWidget(colorLabel_, 2, 2, 1, 2);
	g->setRowStretch(3, 1);
	addWidget(b);

	if (object)
	{
		object->addCallbackParameterChanged(std::tr1::bind(&ParameterObject::objectDidChange, this));
	}
}

ParameterObject::~ParameterObject()
{
	std::tr1::shared_ptr<Object> o = object_.lock();
	if (o)
	{
		o->removeCallbackParameterChanged(this);
	}
}

void ParameterObject::objectDidChange()
{
	std::tr1::shared_ptr<Object> o = object_.lock();
	if (o)
	{
		lineName_->setText(QString::fromStdString(o->name()));
	}
}

}
