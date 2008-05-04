/*
 *  parameterdialog.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 04.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterdialog.h"

#include <QVBoxLayout>

namespace Examination
{

Parameterdialog::Parameterdialog(QWidget * parent) :
	QWidget(parent)
{
	layout_ = new QVBoxLayout();
	setLayout(layout_);
}

Parameterdialog::~Parameterdialog()
{
	delete layout_;
}

QSize Parameterdialog::sizeHint () const
{
	return QSize(256, 256);
}

void Parameterdialog::addWidget(QWidget * w)
{
	layout_->addWidget(w);
}

}
