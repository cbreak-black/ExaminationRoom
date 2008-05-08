/*
 *  parameterdepthbuffer.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 08.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parameterdepthbuffer.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QCheckBox>

#include "objects/depthbuffer.h"

namespace Examination
{

ParameterDepthBuffer::ParameterDepthBuffer(std::tr1::shared_ptr<Object> object) : ParameterObject(object)
{
	QGroupBox * b = new QGroupBox(this);
	QGridLayout * g = new QGridLayout(b);
	b->setTitle(tr("DepthBuffer"));
	b->setLayout(g);
	checkboxDepth_ = new QCheckBox("Depth Buffer", this);
	g->addWidget(checkboxDepth_, 0, 0);
	g->setRowStretch(1, 1);
	addWidget(b);

	connect(checkboxDepth_, SIGNAL(stateChanged(int)),
			this, SLOT(depthStateChanged(int)));

	if (object)
	{
		reloadData();
	}
}

void ParameterDepthBuffer::reloadData()
{
	ParameterObject::reloadData();
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<DepthBuffer> d = std::tr1::dynamic_pointer_cast<DepthBuffer,Object>(o);
		if (d)
		{
			// Depth Buffer State
			checkboxDepth_->setCheckState(d->depthBufferState()? Qt::Checked : Qt::Unchecked);
		}
	}
}

void ParameterDepthBuffer::depthStateChanged(int state)
{
	std::tr1::shared_ptr<Object> o = object();
	if (o)
	{
		std::tr1::shared_ptr<DepthBuffer> d = std::tr1::dynamic_pointer_cast<DepthBuffer,Object>(o);
		if (d)
		{
			d->setDepthBufferState(state == Qt::Checked);
		}
	}
}

}
