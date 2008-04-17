/*
 *  designwidget.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 14.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "designwidget.h"

#include "helper/scenemodel.h"

namespace Examination
{

DesignWidget::DesignWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags)
	: QDockWidget(title, parent, flags)
{
	treeView_ = new QTreeView();
	treeView_->setUniformRowHeights(true);
	setWidget(treeView_);
}

DesignWidget::~DesignWidget()
{
	delete treeView_;
}

// Accessors

std::tr1::shared_ptr<Scene> DesignWidget::scene() const
{
	return scene_;
}

void DesignWidget::setScene(std::tr1::shared_ptr<Scene> scene)
{
	scene_ = scene;
	SceneModel * sm = new SceneModel(scene);
	treeView_->setModel(sm);
	sceneModel_ = std::tr1::shared_ptr<SceneModel>(sm);
	treeView_->setColumnWidth(0, 192);
	treeView_->setColumnWidth(1, 32);
//	treeView_->resizeColumnToContents(0);
//	treeView_->resizeColumnToContents(1);
}


}