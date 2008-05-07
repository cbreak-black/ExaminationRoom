/*
 *  parametertext.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 07.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parametertext.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "objects/text.h"

namespace Examination
{

ParameterText::ParameterText(std::tr1::shared_ptr<Object> object) : ParameterObject(object)
{
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Text"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Text:")), 0, 0);
	lineText_ = new QLineEdit();
	g->addWidget(lineText_, 0, 2, 1, 3);
	g->setRowStretch(1, 1);
	addWidget(b);

	connect(lineText_, SIGNAL(editingFinished()),
			this, SLOT(textEdited()));

	if (object)
	{
		reloadData();
	}
}

void ParameterText::reloadData()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Text> t = std::tr1::dynamic_pointer_cast<Text,Object>(o);
		if (t)
		{
			// Text
			lineText_->setText(QString::fromStdString(t->text()));
		}
	}
}

void ParameterText::textEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Text> t = std::tr1::dynamic_pointer_cast<Text,Object>(o);
		if (t)
		{
			t->setText(lineText_->text().toStdString());
		}
	}
}

}
