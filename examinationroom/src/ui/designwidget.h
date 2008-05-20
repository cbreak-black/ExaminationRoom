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
	class Program;
	class Object;
	class SceneModel;
	class Parameterdialog;

class DesignWidget : public QDockWidget
{
	Q_OBJECT

public:
	DesignWidget(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~DesignWidget();

public:
	std::tr1::shared_ptr<Program> program() const;
	void setProgram(std::tr1::shared_ptr<Program> program);

public:
	/**
	Returns the object that is selected in the tree view, or an invalid pointer.
	 \return the currently selected object
	*/
	std::tr1::shared_ptr<Object> selectedObject();

public slots:
	void editObject(const QModelIndex & current, const QModelIndex & previous);
	void editObject(std::tr1::shared_ptr<Object> object);

	/**
	 Removes the selected object
	 \return true if an object was removed, false otherwise
	 */
	bool removeSelectedObject();

private:
	std::tr1::shared_ptr<Program> program_;
	std::tr1::shared_ptr<SceneModel> sceneModel_;

	QTreeView * treeView_;
	QScrollArea * scrollArea_;
	std::tr1::shared_ptr<Parameterdialog> currentDialog_;
};


}

#endif