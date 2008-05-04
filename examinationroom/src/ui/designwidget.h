/*
 *  designwidget.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 14.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DESIGNWIDGET_H
#define DESIGNWIDGET_H

#include <QDockWidget>

#include <memory>

class QTreeView;
class QScrollArea;

namespace Examination
{
	class Scene;
	class SceneModel;
	class Parameterdialog;

class DesignWidget : public QDockWidget
{
public:
	DesignWidget(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~DesignWidget();

public:
	std::tr1::shared_ptr<Scene> scene() const;
	void setScene(std::tr1::shared_ptr<Scene> scene);

private:
	std::tr1::shared_ptr<Scene> scene_;
	std::tr1::shared_ptr<SceneModel> sceneModel_;

	QTreeView * treeView_;
	QScrollArea * scrollArea_;
	Parameterdialog * currentDialog_;
};


}

#endif