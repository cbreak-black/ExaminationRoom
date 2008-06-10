/*
 *  parametertexture.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 27.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parametertexture.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "surfaces/texture.h"

namespace Examination
{

ParameterTexture::ParameterTexture(std::tr1::shared_ptr<AbstractTexture> texture)
	: ParameterAbstractTexture(texture)
{
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Texture"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Path:")), 0, 0);
	linePath_ = new QLineEdit();
	QPushButton * openTexture = new QPushButton("Open Texture...", this);
	g->addWidget(linePath_, 0, 2, 1, 3);
	g->addWidget(openTexture, 1, 3, 1, 2);
	g->setRowStretch(2, 1);

	addWidget(b);

	connect(linePath_, SIGNAL(editingFinished()),
			this, SLOT(pathEdited()));
	connect(openTexture, SIGNAL(clicked()),
			this, SLOT(openTexture()));

	reloadData();
}

void ParameterTexture::reloadData()
{
	ParameterAbstractTexture::reloadData();
	if (texture())
	{
		std::tr1::shared_ptr<Texture> tex = std::tr1::dynamic_pointer_cast<Texture>(texture());
		if (tex)
		{
			// Path
			linePath_->setText(QString::fromStdString(tex->path()));
		}
	}
}

void ParameterTexture::pathEdited()
{
	if (texture())
	{
		std::tr1::shared_ptr<Texture> tex = std::tr1::dynamic_pointer_cast<Texture>(texture());
		if (tex)
		{
			// Path
			tex->setPath(linePath_->text().toStdString());
			reloadData(); // No auto reload, unlike for objects
		}
	}
}

void ParameterTexture::openTexture()
{
	if (texture())
	{
		std::tr1::shared_ptr<Texture> tex = std::tr1::dynamic_pointer_cast<Texture>(texture());
		if (tex)
		{
			// Path
			std::string p = openFileRelative("Open Texture");
			if (!p.empty())
			{
				tex->setPath(p);
				reloadData(); // No auto reload, unlike for objects
			}
		}
	}
}


}
