/*
 *  mainwindow.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTimer>

#include <memory>

namespace Examination
{
	class Scene;
	class GLWidget;
	class LuaProxy;

/**
The main window. It contains only the GLWidget. It also creates most of the other
objects of the program. A seperate application class was found unnecessary. So far.
 
 \author Gerhard Roethlin
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	/**
	Creates the MainWindow and the environment.
	*/
    MainWindow();
	
	/**
	Destroys the the MainWindow and the environment.
	*/
	~MainWindow();
	
protected:
	virtual void keyPressEvent(QKeyEvent * event);
	virtual void keyReleaseEvent(QKeyEvent * event);

public slots:
	/**
	Opens a file open dialog and loads the selected file as lua script with a new
	environment.
	*/
	void loadLuaFile();

private slots:
	void onTimeout();

private:
    GLWidget * mainGlWidget_;
	GLWidget * outGlWidget_;
	QTimer *timer_;

	QDockWidget * dockDesign_;
	QDockWidget * dockProgram_;
	QDockWidget * dockCode_;

	std::tr1::shared_ptr<Scene> scene_;

	std::tr1::shared_ptr<LuaProxy> luaProxy_;
};

}

#endif
