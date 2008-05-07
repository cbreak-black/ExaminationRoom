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
#include <QTextStream>
#include <QPushButton>
#include <QCheckBox>
#include <QColorDialog>

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
	g->addWidget(new QLabel(tr("Priority:")), 3, 0);
//	g->addWidget(new QLabel(tr("Wireframe:")), 4, 0);
	lineName_ = new QLineEdit();
	linePosX_ = new QLineEdit();
	linePosY_ = new QLineEdit();
	linePosZ_ = new QLineEdit();
	linePriority_ = new QLineEdit();
	linePosX_->setAlignment(Qt::AlignRight);
	linePosY_->setAlignment(Qt::AlignRight);
	linePosZ_->setAlignment(Qt::AlignRight);
	linePriority_->setAlignment(Qt::AlignLeft);
	colorLabel_ = new QLabel();
	colorLabel_->setText("color");
	colorLabel_->setAutoFillBackground(true);
	QPushButton * cb = new QPushButton("Set", this);
	checkboxWireframe_ = new QCheckBox("Wireframe", this);
	g->addWidget(lineName_, 0, 2, 1, 3);
	g->addWidget(linePosX_, 1, 2);
	g->addWidget(linePosY_, 1, 3);
	g->addWidget(linePosZ_, 1, 4);
	g->addWidget(colorLabel_, 2, 2, 1, 2);
	g->addWidget(cb, 2, 4);
	g->addWidget(linePriority_, 3, 2, 1, 3);
	g->addWidget(checkboxWireframe_, 4, 0, 1, 5);
	g->setRowStretch(5, 1);
	addWidget(b);

	connect(lineName_, SIGNAL(editingFinished()),
			this, SLOT(nameEdited()));
	connect(linePosX_, SIGNAL(editingFinished()),
			this, SLOT(posXEdited()));
	connect(linePosY_, SIGNAL(editingFinished()),
			this, SLOT(posYEdited()));
	connect(linePosZ_, SIGNAL(editingFinished()),
			this, SLOT(posZEdited()));
	connect(linePriority_, SIGNAL(editingFinished()),
			this, SLOT(priorityEdited()));
	connect(checkboxWireframe_, SIGNAL(stateChanged(int)),
			this, SLOT(wireframeStateChanged(int)));
	connect(cb, SIGNAL(clicked()),
			this, SLOT(setColor()));

	if (object)
	{
		object->addCallbackParameterChanged(std::tr1::bind(&ParameterObject::objectDidChange, this));
		reloadData();
	}
}

ParameterObject::~ParameterObject()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		o->removeCallbackParameterChanged(this);
	}
}

void ParameterObject::objectDidChange()
{
	reloadData();
}

void ParameterObject::reloadData()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		// Name
		lineName_->setText(QString::fromStdString(o->name()));
		// Position
		Tool::Point p = o->position();
		linePosX_->setText(QString::number(p.x, 'g', 3));
		linePosY_->setText(QString::number(p.y, 'g', 3));
		linePosZ_->setText(QString::number(p.z, 'g', 3));
		// Color
		Tool::Color4 c = o->color();
		QString colorString;
		QTextStream helperStream(&colorString);
		helperStream.setRealNumberPrecision(2);
		helperStream << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
		colorLabel_->setText(colorString);
		colorLabel_->setPalette(QPalette(QColor(255*c.r, 255*c.g, 255*c.b, 255*c.a)));
		// Priority
		linePriority_->setText(QString::number(o->drawPriority()));
		// Wireframe
		checkboxWireframe_->setCheckState(o->wireframe()? Qt::Checked : Qt::Unchecked);
	}
}

std::tr1::shared_ptr<Object> ParameterObject::object() const
{
	return object_.lock();
}

// Slots from the UI
void ParameterObject::nameEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		o->setName(lineName_->text().toStdString());
	}
}

void ParameterObject::posXEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		bool ok = false;
		float f = linePosX_->text().toFloat(&ok);
		if (ok)
		{
			Tool::Point p = o->position();
			p.x = f;
			o->setPosition(p);
		}
	}
}

void ParameterObject::posYEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		bool ok = false;
		float f = linePosY_->text().toFloat(&ok);
		if (ok)
		{
			Tool::Point p = o->position();
			p.y = f;
			o->setPosition(p);
		}
	}
}

void ParameterObject::posZEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		bool ok = false;
		float f = linePosZ_->text().toFloat(&ok);
		if (ok)
		{
			Tool::Point p = o->position();
			p.z = f;
			o->setPosition(p);
		}
	}
}

void ParameterObject::priorityEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		bool ok = false;
		int i = linePriority_->text().toInt(&ok);
		if (ok)
		{
			o->setDrawPriority(i);
		}
	}
}

void ParameterObject::wireframeStateChanged(int state)
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		o->setWireframe(state == Qt::Checked);
	}
}

void ParameterObject::setColor()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		Tool::Color4 c = o->color();
		QColor currentColor = QColor(255*c.r, 255*c.g, 255*c.b, 255*c.a);
		QColor newColor = QColorDialog::getColor(currentColor, this);
		if (newColor.isValid())
		{
			o->setColor(Tool::Color4(newColor.redF(), newColor.greenF(), newColor.blueF(), newColor.alphaF()));
		}
	}
}

}
