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

#include <QWidget>
#include <QTimer>

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
class MainWindow : public QWidget
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

private slots:
	void onTimeout();

private:
    GLWidget * mainGlWidget_;
	GLWidget * outGlWidget_;
	QTimer *timer_;

	Scene * scene_;

	LuaProxy * luaProxy_;
};

}

#endif
