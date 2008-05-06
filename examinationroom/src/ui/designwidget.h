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
class QModelIndex;
class QScrollArea;

namespace Examination
{
	class Object;
	class Scene;
	class SceneModel;
	class Parameterdialog;

class DesignWidget : public QDockWidget
{
	Q_OBJECT

public:
	DesignWidget(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~DesignWidget();

public:
	std::tr1::shared_ptr<Scene> scene() const;
	void setScene(std::tr1::shared_ptr<Scene> scene);

public slots:
	void editObject(const QModelIndex & index);
	void editObject(std::tr1::shared_ptr<Object> object);

private:
	std::tr1::shared_ptr<Scene> scene_;
	std::tr1::shared_ptr<SceneModel> sceneModel_;

	QTreeView * treeView_;
	QScrollArea * scrollArea_;
	std::tr1::shared_ptr<Parameterdialog> currentDialog_;
};


}

#endif