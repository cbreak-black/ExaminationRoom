/*
 *  parameterrectangle.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 07.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parameterrectangle.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "objects/rectangle.h"

namespace Examination
{

ParameterRectangle::ParameterRectangle(std::tr1::shared_ptr<Object> object) : ParameterObject(object)
{
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("Rectangle"));
	b->setLayout(g);
	g->addWidget(new QLabel(tr("Direction A:")), 0, 0);
	g->addWidget(new QLabel(tr("Direction B:")), 1, 0);
	g->addWidget(new QLabel(tr("Subdivision:")), 2, 0);
	g->addWidget(new QLabel(tr("Tex Coords A:")), 3, 0);
	g->addWidget(new QLabel(tr("Tex Coords B:")), 4, 0);
	g->addWidget(new QLabel(tr("Tex Coords C:")), 5, 0);
	g->addWidget(new QLabel(tr("Tex Coords D:")), 6, 0);
	lineDirAX_ = new QLineEdit();
	lineDirAY_ = new QLineEdit();
	lineDirAZ_ = new QLineEdit();
	lineDirBX_ = new QLineEdit();
	lineDirBY_ = new QLineEdit();
	lineDirBZ_ = new QLineEdit();
	lineSubdivision_ = new QLineEdit();
	lineTexAX_ = new QLineEdit();
	lineTexAY_ = new QLineEdit();
	lineTexBX_ = new QLineEdit();
	lineTexBY_ = new QLineEdit();
	lineTexCX_ = new QLineEdit();
	lineTexCY_ = new QLineEdit();
	lineTexDX_ = new QLineEdit();
	lineTexDY_ = new QLineEdit();
	lineDirAX_->setAlignment(Qt::AlignRight);
	lineDirAY_->setAlignment(Qt::AlignRight);
	lineDirAZ_->setAlignment(Qt::AlignRight);
	lineDirBX_->setAlignment(Qt::AlignRight);
	lineDirBY_->setAlignment(Qt::AlignRight);
	lineDirBZ_->setAlignment(Qt::AlignRight);
	lineSubdivision_->setAlignment(Qt::AlignRight);
	lineTexAX_->setAlignment(Qt::AlignRight);
	lineTexAY_->setAlignment(Qt::AlignRight);
	lineTexBX_->setAlignment(Qt::AlignRight);
	lineTexBY_->setAlignment(Qt::AlignRight);
	lineTexCX_->setAlignment(Qt::AlignRight);
	lineTexCY_->setAlignment(Qt::AlignRight);
	lineTexDX_->setAlignment(Qt::AlignRight);
	lineTexDY_->setAlignment(Qt::AlignRight);
	g->addWidget(lineDirAX_, 0, 2);
	g->addWidget(lineDirAY_, 0, 3);
	g->addWidget(lineDirAZ_, 0, 4);
	g->addWidget(lineDirBX_, 1, 2);
	g->addWidget(lineDirBY_, 1, 3);
	g->addWidget(lineDirBZ_, 1, 4);
	g->addWidget(lineSubdivision_, 2, 2, 1, 3);
	g->addWidget(lineTexAX_, 3, 2);
	g->addWidget(lineTexAY_, 3, 3);
	g->addWidget(lineTexBX_, 4, 2);
	g->addWidget(lineTexBY_, 4, 3);
	g->addWidget(lineTexCX_, 5, 2);
	g->addWidget(lineTexCY_, 5, 3);
	g->addWidget(lineTexDX_, 6, 2);
	g->addWidget(lineTexDY_, 6, 3);
	g->setRowStretch(7, 1);
	addWidget(b);

	connect(lineDirAX_, SIGNAL(editingFinished()),
			this, SLOT(dirAEdited()));
	connect(lineDirAY_, SIGNAL(editingFinished()),
			this, SLOT(dirAEdited()));
	connect(lineDirAZ_, SIGNAL(editingFinished()),
			this, SLOT(dirAEdited()));
	connect(lineDirBX_, SIGNAL(editingFinished()),
			this, SLOT(dirBEdited()));
	connect(lineDirBY_, SIGNAL(editingFinished()),
			this, SLOT(dirBEdited()));
	connect(lineDirBZ_, SIGNAL(editingFinished()),
			this, SLOT(dirBEdited()));
	connect(lineSubdivision_, SIGNAL(editingFinished()),
			this, SLOT(subdivisionEdited()));
	connect(lineTexAX_, SIGNAL(editingFinished()),
			this, SLOT(texAEdited()));
	connect(lineTexAY_, SIGNAL(editingFinished()),
			this, SLOT(texAEdited()));
	connect(lineTexBX_, SIGNAL(editingFinished()),
			this, SLOT(texBEdited()));
	connect(lineTexBY_, SIGNAL(editingFinished()),
			this, SLOT(texBEdited()));
	connect(lineTexCX_, SIGNAL(editingFinished()),
			this, SLOT(texCEdited()));
	connect(lineTexCY_, SIGNAL(editingFinished()),
			this, SLOT(texCEdited()));
	connect(lineTexDX_, SIGNAL(editingFinished()),
			this, SLOT(texDEdited()));
	connect(lineTexDY_, SIGNAL(editingFinished()),
			this, SLOT(texDEdited()));

	if (object)
	{
		reloadData();
	}
}

void ParameterRectangle::reloadData()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Rectangle> r = std::tr1::dynamic_pointer_cast<Rectangle,Object>(o);
		if (r)
		{
			// Dir A
			Tool::Vec3f dA = r->dirA();
			lineDirAX_->setText(QString::number(dA.x,'g',3));
			lineDirAY_->setText(QString::number(dA.y,'g',3));
			lineDirAZ_->setText(QString::number(dA.z,'g',3));
			// Dir B
			Tool::Vec3f dB = r->dirB();
			lineDirBX_->setText(QString::number(dB.x,'g',3));
			lineDirBY_->setText(QString::number(dB.y,'g',3));
			lineDirBZ_->setText(QString::number(dB.z,'g',3));
			// Subdivision
			lineSubdivision_->setText(QString::number(r->subdivision()));
			// Tex A
			Tool::Vec2f texPoint = r->texA();
			lineTexAX_->setText(QString::number(texPoint.x,'g',3));
			lineTexAY_->setText(QString::number(texPoint.y,'g',3));
			// Tex B
			texPoint = r->texB();
			lineTexBX_->setText(QString::number(texPoint.x,'g',3));
			lineTexBY_->setText(QString::number(texPoint.y,'g',3));
			// Tex C
			texPoint = r->texC();
			lineTexCX_->setText(QString::number(texPoint.x,'g',3));
			lineTexCY_->setText(QString::number(texPoint.y,'g',3));
			// Tex D
			texPoint = r->texD();
			lineTexDX_->setText(QString::number(texPoint.x,'g',3));
			lineTexDY_->setText(QString::number(texPoint.y,'g',3));
		}
	}
}

void ParameterRectangle::dirAEdited()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Rectangle> r = std::tr1::dynamic_pointer_cast<Rectangle,Object>(o);
		if (r)
		{
			bool okX, okY, okZ;
			float fx = lineDirAX_->text().toFloat(&okX);
			float fy = lineDirAY_->text().toFloat(&okY);
			float fz = lineDirAZ_->text().toFloat(&okZ);
			if (okX && okY && okZ)
			{
				r->setDirA(Tool::Vec3f(fx, fy, fz));
			}
			else
			{
				reloadData();
			}
		}
	}
}

void ParameterRectangle::dirBEdited()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Rectangle> r = std::tr1::dynamic_pointer_cast<Rectangle,Object>(o);
		if (r)
		{
			bool okX, okY, okZ;
			float fx = lineDirBX_->text().toFloat(&okX);
			float fy = lineDirBY_->text().toFloat(&okY);
			float fz = lineDirBZ_->text().toFloat(&okZ);
			if (okX && okY && okZ)
			{
				r->setDirB(Tool::Vec3f(fx, fy, fz));
			}
			else
			{
				reloadData();
			}
		}
	}
}

void ParameterRectangle::subdivisionEdited()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Rectangle> r = std::tr1::dynamic_pointer_cast<Rectangle,Object>(o);
		if (r)
		{
			bool ok;
			int i = lineSubdivision_->text().toInt(&ok);
			if (ok)
			{
				r->setSubdivision(i);
			}
			else
			{
				reloadData();
			}
		}
	}
}

void ParameterRectangle::texAEdited()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Rectangle> r = std::tr1::dynamic_pointer_cast<Rectangle,Object>(o);
		if (r)
		{
			bool okX, okY;
			float fx = lineTexAX_->text().toFloat(&okX);
			float fy = lineTexAY_->text().toFloat(&okY);
			if (okX && okY)
			{
				r->setTexA(Tool::Vec2f(fx, fy));
			}
			else
			{
				reloadData();
			}
		}
	}
}

void ParameterRectangle::texBEdited()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Rectangle> r = std::tr1::dynamic_pointer_cast<Rectangle,Object>(o);
		if (r)
		{
			bool okX, okY;
			float fx = lineTexBX_->text().toFloat(&okX);
			float fy = lineTexBY_->text().toFloat(&okY);
			if (okX && okY)
			{
				r->setTexB(Tool::Vec2f(fx, fy));
			}
			else
			{
				reloadData();
			}
		}
	}
}

void ParameterRectangle::texCEdited()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Rectangle> r = std::tr1::dynamic_pointer_cast<Rectangle,Object>(o);
		if (r)
		{
			bool okX, okY;
			float fx = lineTexCX_->text().toFloat(&okX);
			float fy = lineTexCY_->text().toFloat(&okY);
			if (okX && okY)
			{
				r->setTexC(Tool::Vec2f(fx, fy));
			}
			else
			{
				reloadData();
			}
		}
	}
}

void ParameterRectangle::texDEdited()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<Rectangle> r = std::tr1::dynamic_pointer_cast<Rectangle,Object>(o);
		if (r)
		{
			bool okX, okY;
			float fx = lineTexDX_->text().toFloat(&okX);
			float fy = lineTexDY_->text().toFloat(&okY);
			if (okX && okY)
			{
				r->setTexD(Tool::Vec2f(fx, fy));
			}
			else
			{
				reloadData();
			}
		}
	}
}

}
