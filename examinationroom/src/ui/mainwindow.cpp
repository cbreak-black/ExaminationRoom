/*
 *  mainwindow.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include <QtGui>

#include "mainwindow.h"

#include "glwidget.h"
#include "designwidget.h"

#include "program.h"

#include <iostream>
#include <fstream>

namespace Examination
{

MainWindow::MainWindow()
{
	// Set Title and window properties
	setWindowTitle(tr("Examinationroom"));
	QMainWindow::setDockOptions(AnimatedDocks | AllowNestedDocks | AllowTabbedDocks);

	// Create GL Widgets
	QGLFormat glFormat = QGLFormat::defaultFormat();
	// Stereo Buffering seems to disable other things, like double buffering
	//glFormat.setStereo(true);
	QGLFormat::setDefaultFormat(glFormat);

	mainGlWidget_ = new GLWidget(this);
//	outGlWidget_ = new GLWidget(0, mainGlWidget_);
	mainGlWidget_->setStyle(GLWidget::single);
//	outGlWidget_->setStyle(GLWidget::single);

	// Add menu
	QMenu *menu = menuBar()->addMenu(tr("&File"));
	menu->addAction(tr("&Open Scene..."),
					this, SLOT(loadLuaFile()),
					QKeySequence(QKeySequence::Open));
	menu->addAction(tr("Close Scene"),
					this, SLOT(newScene()),
					QKeySequence(QKeySequence::Close));
	menu->addAction(tr("&Save Scene..."),
					this, SLOT(storeLuaFile()),
					QKeySequence(QKeySequence::Save));
#ifndef Q_WS_MACX
	menu->addSeparator();
	menu->addAction(tr("&Quit"), this, SLOT(close()),
					QKeySequence(tr("ctrl-Q")));
#endif

	// Signal mapper for display types
	signalMapper_ = new QSignalMapper(this);

	QAction * action;
	menu = menuBar()->addMenu(tr("&Output Mode"));
	action = menu->addAction(tr("&Anaglyph"));
	action->setShortcut(QKeySequence(Qt::Key_1 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::anaglyph);

	action = menu->addAction(tr("&Side by Side"));
	action->setShortcut(QKeySequence(Qt::Key_2 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::sidebyside);

	action = menu->addAction(tr("&Line interlacing (Experimental)"));
	action->setShortcut(QKeySequence(Qt::Key_3 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::line);

	action = menu->addAction(tr("Quad Buffering (Experimental)"));
	action->setShortcut(QKeySequence(Qt::Key_4 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::quad);

	action = menu->addAction(tr("Matrix Anaglyph (Experimental)"));
	action->setShortcut(QKeySequence(Qt::Key_5 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::matrix);

	menu->addSeparator();

	action = menu->addAction(tr("Si&ngle"));
	action->setShortcut(QKeySequence(Qt::Key_0 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::single);

	connect(signalMapper_, SIGNAL(mapped(int)), this, SLOT(setDrawStyle(int)));

	// Add Dock Widgets
	dockDesign_ = new DesignWidget("Scene Design", this);
	dockProgram_ = new QDockWidget("Scene Program", this);
	dockCode_ = new QDockWidget("Code", this);
	dockDesign_->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	dockProgram_->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	dockCode_->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	addDockWidget(Qt::RightDockWidgetArea, dockDesign_);
	addDockWidget(Qt::RightDockWidgetArea, dockProgram_);
	addDockWidget(Qt::RightDockWidgetArea, dockCode_);
	tabifyDockWidget(dockDesign_, dockProgram_);
	tabifyDockWidget(dockDesign_, dockCode_);

	// Set up layout
	setFocusPolicy(Qt::StrongFocus);
	setCentralWidget(mainGlWidget_);

//	int numScreens = QApplication::desktop()->numScreens();

//	if (numScreens == 2)
//	{
//		// Two Screens used by output widget
//		outGlWidget_->setStyle(GLWidget::sidebyside);
//		outGlWidget_->showFullScreen();
//		QRect s2 = QApplication::desktop()->screenGeometry(0);
//		s2 = s2.united(QApplication::desktop()->screenGeometry(1));
//		outGlWidget_->setGeometry(s2);
//	}
//	if (numScreens == 1)
//	{
//		outGlWidget_->setStyle(GLWidget::single);
//		outGlWidget_->showFullScreen();
//	}

	// Create Scene
	setProgram(Program::create());

	// Set up redraw timer
	timer_ = new QTimer(this);
	connect(timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));
	timer_->start(33); // 30 fps
}


MainWindow::~MainWindow()
{
	// All child widgets are deallocated by QT
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Escape)
	{
		QCoreApplication::quit();
	}
	else
	{
		program_->onKeyDown(event->key());
		mainGlWidget_->update(); // update() for deferred updates
//		outGlWidget_->update();
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
	program_->onKeyUp(event->key());
	mainGlWidget_->update(); // update() for deferred updates
//	outGlWidget_->update();
}

void MainWindow::onTimeout()
{
	program_->onUpdate();
	mainGlWidget_->update(); // update() for deferred updates
//	outGlWidget_->update();
}

void MainWindow::loadLuaFile()
{
	// Set up lua engine and bindings
	QString fileName = QFileDialog::getOpenFileName(this, "Open LUA File",
													"res/scene.lua",
													"Lua Scene File (*.lua)");
	if (!fileName.isNull())
	{
		setProgram(Program::createFromLua(fileName.toStdString()));
	}
}

void MainWindow::storeLuaFile()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save LUA File",
													"res/",
													"Lua Scene File (*.lua)");
	if (!fileName.isNull())
	{
		std::ofstream of;
		of.open(fileName.toAscii(), std::ios_base::out);
		program_->toLua(of);
	}
}

void MainWindow::setDrawStyle(int t)
{
	mainGlWidget_->setStyle((GLWidget::DrawStyle)t);
}

void MainWindow::newScene()
{
	setProgram(Program::create());
}

void MainWindow::setProgram(std::tr1::shared_ptr<Program> program)
{
	program_ = program;
	mainGlWidget_->setScene(program_->scene());
	//outGlWidget_->setScene(program_->scene());
	dockDesign_->setScene(program_->scene());
}


}
