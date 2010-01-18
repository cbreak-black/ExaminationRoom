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
	g->addWidget(lineText_, 0, 2, 1, 2);
	g->addWidget(new QLabel(tr("Dimension:")), 1, 0);
	lineDimX_ = new QLineEdit();
	lineDimY_ = new QLineEdit();
	g->addWidget(lineDimX_, 1, 2, 1, 1);
	g->addWidget(lineDimY_, 1, 3, 1, 1);
	g->setRowStretch(1, 1);
	addWidget(b);

	connect(lineText_, SIGNAL(editingFinished()),
			this, SLOT(textEdited()));
	connect(lineDimX_, SIGNAL(editingFinished()),
			this, SLOT(dimEdited()));
	connect(lineDimY_, SIGNAL(editingFinished()),
			this, SLOT(dimEdited()));

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
			// Dimensions
			Tool::Vec2f d = t->dimensions();
			lineDimX_->setText(QString::number(d.w, 'g', 3));
			lineDimY_->setText(QString::number(d.h, 'g', 3));
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

void ParameterText::dimEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Text> t = std::tr1::dynamic_pointer_cast<Text,Object>(o);
		if (t)
		{
			Tool::Vec2f d = t->dimensions();
			bool ok = false;
			float f;
			f = lineDimX_->text().toFloat(&ok);
			if (ok)
				d.w = f;
			f = lineDimY_->text().toFloat(&ok);
			if (ok)
				d.h = f;
			t->setDimensions(d);
		}
	}
}

}
