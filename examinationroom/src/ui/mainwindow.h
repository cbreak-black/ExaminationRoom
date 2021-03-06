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
class QFileDialog;

namespace Examination
{
	class AboutWindow;
	class GLWidget;
	class DesignWidget;
	class CodeWidget;
	class ConsoleWidget;
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

public slots:
	/**
	Loads the passed file.
	*/
	void loadLuaFile(const QString & fileName);

	/**
	Closes the current scene and creates a new one in it's place
	*/
	void newScene();

	/**
	Stores the current scene in the file with the passed name.
	*/
	void storeLuaFile(const QString & fileName);

	/**
	Reverts the current scene to the last saved state.
	*/
	void revert();

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
	ConsoleWidget * dockConsole_;
	AboutWindow * aboutWindow_;

	QFileDialog * loadDialog_;
	QFileDialog * storeDialog_;

	std::tr1::shared_ptr<Program> program_;
};

}

#endif
