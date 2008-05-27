/*
 *  parameterpatternstereogram.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 27.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterpatternstereogram.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "surfaces/patternstereogram.h"
#include "surfaces/texture.h"

namespace Examination
{

ParameterPatternStereogram::ParameterPatternStereogram(std::tr1::shared_ptr<AbstractTexture> texture)
	: ParameterStereogram(texture)
{
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Pattern"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Background:")), 0, 0);
	g->addWidget(new QLabel(tr("Foreground:")), 1, 0);
	linePathBG_ = new QLineEdit();
	linePathFG_ = new QLineEdit();
	g->addWidget(linePathBG_, 0, 2, 1, 3);
	g->addWidget(linePathFG_, 1, 2, 1, 3);
	g->setRowStretch(2, 1);

	addWidget(b);

	connect(linePathBG_, SIGNAL(editingFinished()),
			this, SLOT(pathBGEdited()));
	connect(linePathFG_, SIGNAL(editingFinished()),
			this, SLOT(pathFGEdited()));

	reloadData();
}

void ParameterPatternStereogram::reloadData()
{
	ParameterStereogram::reloadData();
	if (texture())
	{
		std::tr1::shared_ptr<PatternStereogram> tex = std::tr1::dynamic_pointer_cast<PatternStereogram>(texture());
		if (tex)
		{
			// Paths
			linePathBG_->setText(QString::fromStdString(tex->texBG()->path()));
			linePathFG_->setText(QString::fromStdString(tex->texFG()->path()));
		}
	}
}

void ParameterPatternStereogram::pathBGEdited()
{
	if (texture())
	{
		std::tr1::shared_ptr<PatternStereogram> tex = std::tr1::dynamic_pointer_cast<PatternStereogram>(texture());
		if (tex)
		{
			// Path
			tex->texBG()->setPath(linePathBG_->text().toStdString());
			tex->recreateStereogram();
			reloadData(); // No auto reload, unlike for objects
		}
	}
}

void ParameterPatternStereogram::pathFGEdited()
{
	if (texture())
	{
		std::tr1::shared_ptr<PatternStereogram> tex = std::tr1::dynamic_pointer_cast<PatternStereogram>(texture());
		if (tex)
		{
			// Path
			tex->texFG()->setPath(linePathFG_->text().toStdString());
			tex->recreateStereogram();
			reloadData(); // No auto reload, unlike for objects
		}
	}
}

}
