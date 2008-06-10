/*
 *  parameteraffinetransformation.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 08.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parameteraffinetransformation.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "objects/affinetransformation.h"

namespace Examination
{

ParameterAffineTransformation::ParameterAffineTransformation(std::tr1::shared_ptr<Object> object)
	: ParameterObject(object)
{
	// Create Object interface
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Affine Transformation"));
	b->setLayout(g);
	g->setRowStretch(0, 1);
	addWidget(b);

	if (object)
	{
		reloadData();
	}
}

void ParameterAffineTransformation::reloadData()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<AffineTransformation> a = std::tr1::dynamic_pointer_cast<AffineTransformation,Object>(o);
		if (a)
		{
		}
	}
}

// Slots from the UI

}
