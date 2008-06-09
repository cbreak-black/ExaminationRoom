/*
 *  parametermesh.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 08.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parametermesh.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "objects/mesh.h"

namespace Examination
{

ParameterMesh::ParameterMesh(std::tr1::shared_ptr<Object> object) : ParameterObject(object)
{
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Mesh"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Path:")), 0, 0);
	g->addWidget(new QLabel(tr("Scale:")), 2, 0);
	linePath_ = new QLineEdit();
	QPushButton * loadMesh = new QPushButton("Load Mesh...", this);
	lineScale_ = new QLineEdit();
	lineScale_->setAlignment(Qt::AlignRight);
	g->addWidget(linePath_, 0, 2, 1, 3);
	g->addWidget(loadMesh, 1, 3, 1, 2);
	g->addWidget(lineScale_, 2, 2, 1, 3);
	g->setRowStretch(3, 1);
	addWidget(b);

	connect(linePath_, SIGNAL(editingFinished()),
			this, SLOT(pathEdited()));
	connect(loadMesh, SIGNAL(clicked()),
			this, SLOT(openMesh()));
	connect(lineScale_, SIGNAL(editingFinished()),
			this, SLOT(scaleEdited()));

	if (object)
	{
		reloadData();
	}
}

void ParameterMesh::reloadData()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Mesh> m = std::tr1::dynamic_pointer_cast<Mesh,Object>(o);
		if (m)
		{
			// Path
			linePath_->setText(QString::fromStdString(m->getMeshPath()));
			// Scale Factor
			lineScale_->setText(QString::number(m->scaleFactor(), 'g', 3));
		}
	}
}

void ParameterMesh::pathEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Mesh> m = std::tr1::dynamic_pointer_cast<Mesh,Object>(o);
		if (m)
		{
			m->loadMesh(linePath_->text().toStdString());
		}
	}
}

void ParameterMesh::scaleEdited()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Mesh> m = std::tr1::dynamic_pointer_cast<Mesh,Object>(o);
		if (m)
		{
			bool ok;
			float f = lineScale_->text().toFloat(&ok);
			if (ok)
			{
				m->setScaleFactor(f);
			}
		}
	}
}

void ParameterMesh::openMesh()
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Mesh> m = std::tr1::dynamic_pointer_cast<Mesh,Object>(o);
		if (m)
		{
			std::string p = openFileRelative("Open Mesh", "Object File (*.obj)");
			if (!p.empty())
			{
				linePath_->setText(QString::fromStdString(p));
				m->loadMesh(p);
			}
		}
	}
}

}
