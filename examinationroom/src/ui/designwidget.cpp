/*
 *  designwidget.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 14.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "designwidget.h"

#include <QTreeView>
#include <QSplitter>
#include <QScrollArea>

#include "program.h"
#include "helper/scenemodel.h"
#include "parameter/parameterobject.h"
#include "objects/object.h"

namespace Examination
{

DesignWidget::DesignWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags)
	: QDockWidget(title, parent, flags)
{
	QSplitter * splitter = new QSplitter(Qt::Vertical, this);
	setWidget(splitter);

	treeView_ = new QTreeView();
	treeView_->setUniformRowHeights(true);
	splitter->addWidget(treeView_);
	scrollArea_ = new QScrollArea(this);
	scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea_->setWidgetResizable(true);
	scrollArea_->setMaximumWidth(512);
	splitter->addWidget(scrollArea_);
}

DesignWidget::~DesignWidget()
{
	scrollArea_->takeWidget(); // Prevent scrollArea_ from deleting the widget
	delete treeView_;
}

void DesignWidget::editObject(const QModelIndex & current, const QModelIndex & /* previous */)
{
	Object * o = static_cast<Object*>(current.internalPointer());
	editObject(o->sharedPtr());
}

void DesignWidget::editObject(std::tr1::shared_ptr<Object> object)
{
	if (object)
	{
		scrollArea_->takeWidget(); // Prevent scrollArea_ from deleting the widget
		currentDialog_ = object->dialog();
		scrollArea_->setWidget(currentDialog_.get());
	}
}

// Accessors

std::tr1::shared_ptr<Program> DesignWidget::program() const
{
	return program_;
}

void DesignWidget::setProgram(std::tr1::shared_ptr<Program> program)
{
	program_ = program;
	// Remove previous widget
	scrollArea_->takeWidget();
	currentDialog_.reset();
	// Set new scene / model
	SceneModel * sm = new SceneModel(program->scene());
	treeView_->setModel(sm);
	sceneModel_ = std::tr1::shared_ptr<SceneModel>(sm);
	treeView_->setColumnWidth(0, 192);
	treeView_->setColumnWidth(1, 48);
	treeView_->setColumnWidth(2, 48);
//	treeView_->resizeColumnToContents(0);
//	treeView_->resizeColumnToContents(1);
	QItemSelectionModel * selection = treeView_->selectionModel();
	connect(selection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(editObject(const QModelIndex &, const QModelIndex &)));
}

std::tr1::shared_ptr<Object> DesignWidget::selectedObject()
{
	QItemSelectionModel * selection = treeView_->selectionModel();
	Object * o = static_cast<Object*>(selection->currentIndex().internalPointer());
	if (o)
		return o->sharedPtr();
	else
		return std::tr1::shared_ptr<Object>();
}

}