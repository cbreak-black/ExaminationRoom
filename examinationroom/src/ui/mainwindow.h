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

#include <memory>

class QMenu;
class QSignalMapper;
class QDockWidget;
class QTimer;

namespace Examination
{
	class AboutWindow;
	class GLWidget;
	class DesignWidget;
	class CodeWidget;
	class LogWidget;
	class Program;

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

	/**
	Closes the current scene and creates a new one in it's place
	*/
	void newScene();

	/**
	Opens a file save dialog and stores sthe current scene as the selected file
	as lua script. If the original was loades from a file, information WILL be lost.
	*/
	void storeLuaFile();

	/**
	Sets the draw type of the main window output view.
	 \param t	A valid GLWidget::DrawStyle integer
	*/
	void setDrawStyle(int t);

	/**
	Toggles full screen mode.
	*/
	void toggleFullscreen();

private slots:
	void onTimeout();
	void onUpdate();
	void onObjectCreate(int id);
	void showAbout();

private:
	void setProgram(std::tr1::shared_ptr<Program> program);

private:
    GLWidget * mainGlWidget_;
	GLWidget * fsGlWidget_;
	QTimer *timer_;
	QSignalMapper * signalMapper_;
	QSignalMapper * objectMapper_;
	QMenu * objectMenu_;

	bool fullScreen_;

	DesignWidget * dockDesign_;
	CodeWidget * dockCode_;
	LogWidget * dockLog_;
	AboutWindow * aboutWindow_;

	std::tr1::shared_ptr<Program> program_;
};

}

#endif
