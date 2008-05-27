/*
 *  parameterrandomdot.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 27.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterrandomdot.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "surfaces/randomdotstereogram.h"
#include "surfaces/texture.h"

namespace Examination
{

ParameterRandomDot::ParameterRandomDot(std::tr1::shared_ptr<AbstractTexture> texture)
	: ParameterStereogram(texture)
{
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Random Dot"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Max Colors:")), 0, 0);
	g->addWidget(new QLabel(tr("Exclusive Colors:")), 1, 0);
	lineMaxColors_ = new QLineEdit();
	lineExcColors_ = new QLineEdit();
	g->addWidget(lineMaxColors_, 0, 2, 1, 3);
	g->addWidget(lineExcColors_, 1, 2, 1, 3);
	g->setRowStretch(2, 1);

	addWidget(b);

	connect(lineMaxColors_, SIGNAL(editingFinished()),
			this, SLOT(maxColorsEdited()));
	connect(lineExcColors_, SIGNAL(editingFinished()),
			this, SLOT(excColorsEdited()));

	reloadData();
}

void ParameterRandomDot::reloadData()
{
	ParameterStereogram::reloadData();
	if (texture())
	{
		std::tr1::shared_ptr<RandomdotStereogram> tex = std::tr1::dynamic_pointer_cast<RandomdotStereogram>(texture());
		if (tex)
		{
			lineMaxColors_->setText(QString::number(tex->maxColor()));
			lineExcColors_->setText(QString::number(tex->exclusiveColor()));
		}
	}
}

void ParameterRandomDot::maxColorsEdited()
{
	if (texture())
	{
		std::tr1::shared_ptr<RandomdotStereogram> tex = std::tr1::dynamic_pointer_cast<RandomdotStereogram>(texture());
		if (tex)
		{
			bool ok;
			int i = lineMaxColors_->text().toInt(&ok);
			if (ok)
			{
				tex->setMaxColor(i);
			}
			reloadData(); // No auto reload, unlike for objects
		}
	}
}

void ParameterRandomDot::excColorsEdited()
{
	if (texture())
	{
		std::tr1::shared_ptr<RandomdotStereogram> tex = std::tr1::dynamic_pointer_cast<RandomdotStereogram>(texture());
		if (tex)
		{
			bool ok;
			int i = lineExcColors_->text().toInt(&ok);
			if (ok)
			{
				tex->setExclusiveColor(i);
			}
			reloadData(); // No auto reload, unlike for objects
		}
	}
}


}
