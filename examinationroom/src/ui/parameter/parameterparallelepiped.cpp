/*
 *  parameterparallelepiped.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 07.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parameterparallelepiped.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "objects/parallelepiped.h"

namespace Examination
{

ParameterParallelepiped::ParameterParallelepiped(std::tr1::shared_ptr<Object> object) : ParameterRectangle(object)
{
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Parallelepiped"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Direction C:")), 0, 0);
	lineDirCX_ = new QLineEdit();
	lineDirCY_ = new QLineEdit();
	lineDirCZ_ = new QLineEdit();
	lineDirCX_->setAlignment(Qt::AlignRight);
	lineDirCY_->setAlignment(Qt::AlignRight);
	lineDirCZ_->setAlignment(Qt::AlignRight);
	g->addWidget(lineDirCX_, 0, 2);
	g->addWidget(lineDirCY_, 0, 3);
	g->addWidget(lineDirCZ_, 0, 4);
	g->setRowStretch(1, 1);
	addWidget(b);

	connect(lineDirCX_, SIGNAL(editingFinished()),
			this, SLOT(dirCEdited()));
	connect(lineDirCY_, SIGNAL(editingFinished()),
			this, SLOT(dirCEdited()));
	connect(lineDirCZ_, SIGNAL(editingFinished()),
			this, SLOT(dirCEdited()));

	if (object)
	{
		reloadData();
	}
}

void ParameterParallelepiped::reloadData()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Parallelepiped> p = std::tr1::dynamic_pointer_cast<Parallelepiped,Object>(o);
		if (p)
		{
			// Dir C
			Tool::Vec3f dC = p->dirC();
			lineDirCX_->setText(QString::number(dC.x,'g',3));
			lineDirCY_->setText(QString::number(dC.y,'g',3));
			lineDirCZ_->setText(QString::number(dC.z,'g',3));
		}
	}
}

void ParameterParallelepiped::dirCEdited()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Parallelepiped> p = std::tr1::dynamic_pointer_cast<Parallelepiped,Object>(o);
		if (p)
		{
			bool okX, okY, okZ;
			float fx = lineDirCX_->text().toFloat(&okX);
			float fy = lineDirCY_->text().toFloat(&okY);
			float fz = lineDirCZ_->text().toFloat(&okZ);
			if (okX && okY && okZ)
			{
				p->setDirC(Tool::Vec3f(fx, fy, fz));
			}
			else
			{
				reloadData();
			}
		}
	}
}

}
