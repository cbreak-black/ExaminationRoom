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

#include "helper/scenemodel.h"
#include "parameter/parameterobject.h"

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
	currentDialog_ = new ParameterObject(std::tr1::shared_ptr<Object>());
	scrollArea_->setWidget(currentDialog_);
}

DesignWidget::~DesignWidget()
{
	currentDialog_ = 0; // Ownership resides with object
	scrollArea_->takeWidget(); // Prevent scrollArea_ from deleting the widget
	delete treeView_;
}

// Accessors

std::tr1::shared_ptr<Scene> DesignWidget::scene() const
{
	return scene_;
}

void DesignWidget::setScene(std::tr1::shared_ptr<Scene> scene)
{
	SceneModel * sm = new SceneModel(scene);
	treeView_->setModel(sm);
	sceneModel_ = std::tr1::shared_ptr<SceneModel>(sm);
	scene_ = scene;
	treeView_->setColumnWidth(0, 192);
	treeView_->setColumnWidth(1, 48);
	treeView_->setColumnWidth(2, 48);
//	treeView_->resizeColumnToContents(0);
//	treeView_->resizeColumnToContents(1);
}


}