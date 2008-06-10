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
#include <QPushButton>

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
	pbBG_ = new QPushButton("...");
	pbFG_ = new QPushButton("...");
	g->addWidget(linePathBG_, 0, 2, 1, 2);
	g->addWidget(linePathFG_, 1, 2, 1, 2);
	g->addWidget(pbBG_, 0, 4);
	g->addWidget(pbFG_, 1, 4);
	g->setRowStretch(2, 1);

	addWidget(b);

	connect(linePathBG_, SIGNAL(editingFinished()),
			this, SLOT(pathBGEdited()));
	connect(linePathFG_, SIGNAL(editingFinished()),
			this, SLOT(pathFGEdited()));
	connect(pbBG_, SIGNAL(clicked()),
			this, SLOT(openBG()));
	connect(pbFG_, SIGNAL(clicked()),
			this, SLOT(openFG()));

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
			if (tex->texBG())
			{
				linePathBG_->setText(QString::fromStdString(tex->texBG()->path()));
			}
			else
			{
				linePathBG_->setEnabled(false);
				pbBG_->setEnabled(false);
			}
			if (tex->texFG())
			{
				linePathFG_->setText(QString::fromStdString(tex->texFG()->path()));
			}
			else
			{
				linePathFG_->setEnabled(false);
				pbFG_->setEnabled(false);
			}
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

void ParameterPatternStereogram::openBG()
{
	if (texture())
	{
		std::tr1::shared_ptr<PatternStereogram> tex = std::tr1::dynamic_pointer_cast<PatternStereogram>(texture());
		if (tex && tex->texBG())
		{
			// Path
			std::string p = openFileRelative("Open Background Texture");
			if (!p.empty())
			{
				tex->texBG()->setPath(p);
				tex->recreateStereogram();
				reloadData(); // No auto reload, unlike for objects
			}
		}
	}
}

void ParameterPatternStereogram::openFG()
{
	if (texture())
	{
		std::tr1::shared_ptr<PatternStereogram> tex = std::tr1::dynamic_pointer_cast<PatternStereogram>(texture());
		if (tex && tex->texFG())
		{
			// Path
			std::string p = openFileRelative("Open Foreground Texture");
			if (!p.empty())
			{
				tex->texFG()->setPath(p);
				tex->recreateStereogram();
				reloadData(); // No auto reload, unlike for objects
			}
		}
	}
}

}
