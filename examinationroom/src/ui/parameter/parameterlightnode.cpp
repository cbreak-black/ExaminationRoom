/*
 *  parameterlightnode.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 08.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parameterlightnode.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextStream>
#include <QColorDialog>

#include "objects/lightnode.h"

namespace Examination
{

ParameterLightNode::ParameterLightNode(std::tr1::shared_ptr<Object> object)
	: ParameterObject(object)
{
	// Create Object interface
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("LightNode"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Ambient:")), 0, 0);
	ambientLabel_ = new QLabel();
	ambientLabel_->setText("color");
	ambientLabel_->setAutoFillBackground(true);
	QPushButton * cb = new QPushButton("Set", this);
	g->addWidget(ambientLabel_, 0, 2, 1, 2);
	g->addWidget(cb, 0, 4);
	g->setRowStretch(1, 1);
	addWidget(b);

	connect(cb, SIGNAL(clicked()),
			this, SLOT(setAmbient()));

	if (object)
	{
		reloadData();
	}
}

void ParameterLightNode::reloadData()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<LightNode> l = std::tr1::dynamic_pointer_cast<LightNode,Object>(o);
		if (l)
		{
			// Ambient
			Tool::Color4 c = l->ambient();
			QString colorString;
			QTextStream helperStream(&colorString);
			helperStream.setRealNumberPrecision(2);
			helperStream << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
			ambientLabel_->setText(colorString);
			ambientLabel_->setPalette(QPalette(QColor(255*c.r, 255*c.g, 255*c.b, 255*c.a)));
		}
	}
}

// Slots from the UI
void ParameterLightNode::setAmbient()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<LightNode> l = std::tr1::dynamic_pointer_cast<LightNode,Object>(o);
		if (l)
		{
			Tool::Color4 c = l->ambient();
			QColor currentColor = QColor(255*c.r, 255*c.g, 255*c.b, 255*c.a);
			QColor newColor = QColorDialog::getColor(currentColor, this);
			if (newColor.isValid())
			{
				l->setAmbient(Tool::Color4(newColor.redF(), newColor.greenF(), newColor.blueF(), newColor.alphaF()));
			}
		}
	}
}

}
