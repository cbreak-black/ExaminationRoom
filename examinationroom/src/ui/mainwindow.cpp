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
#include "scene.h"

#include "designwidget.h"

#include "luaproxy.h"

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
	QAction *action = menu->addAction(tr("&Open Scene..."));
	connect(action, SIGNAL(triggered()), this, SLOT(loadLuaFile()));
	action = menu->addAction(tr("Close Scene"));
	connect(action, SIGNAL(triggered()), this, SLOT(closeScene()));
#ifndef Q_WS_MACX
	menu->addSeparator();
	menu->addAction(tr("&Quit"), this, SLOT(close()));
#endif

	// Signal mapper for display types
	signalMapper_ = new QSignalMapper(this);

	menu = menuBar()->addMenu(tr("&Output Mode"));
	action = menu->addAction(tr("&Anaglyph"));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::anaglyph);
	action = menu->addAction(tr("&Side by Side"));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::sidebyside);
	action = menu->addAction(tr("&Line interlacing (Experimental)"));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::line);
	action = menu->addAction(tr("Quad Buffering (Experimental)"));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::quad);
	action = menu->addAction(tr("Matrix Anaglyph (Experimental)"));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::matrix);
	action = menu->addAction(tr("Si&ngle"));
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
	scene_ = std::tr1::shared_ptr<Scene>(new Scene());
	mainGlWidget_->setScene(scene_);
	//outGlWidget_->setScene(scene_);

	dockDesign_->setScene(scene_);

	// Set up redraw timer
	timer_ = new QTimer(this);
	connect(timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));
	timer_->start(33); // 30 fps

	// Empty lua proxy
	luaProxy_ = std::tr1::shared_ptr<LuaProxy>(new LuaProxy(scene_));
}


MainWindow::~MainWindow()
{
	delete timer_;
	delete signalMapper_;
	delete dockDesign_;
	delete dockProgram_;
	delete dockCode_;
//	delete outGlWidget_;
	delete mainGlWidget_;
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Escape)
	{
		QCoreApplication::quit();
	}
	else
	{
		luaProxy_->onKeyDown(event->key());
		mainGlWidget_->update(); // update() for deferred updates
//		outGlWidget_->update();
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
	luaProxy_->onKeyUp(event->key());
	mainGlWidget_->update(); // update() for deferred updates
//	outGlWidget_->update();
}

void MainWindow::onTimeout()
{
	luaProxy_->onUpdate();
	mainGlWidget_->update(); // update() for deferred updates
//	outGlWidget_->update();
}

void MainWindow::loadLuaFile()
{
	// Set up lua engine and bindings
	QString fileName = QFileDialog::getOpenFileName(this, "Open Log File",
													"res/scene.lua",
													"Lua Scene File (*.lua)");
	if (!fileName.isNull())
	{
		scene_->clear();
		luaProxy_ = std::tr1::shared_ptr<LuaProxy>(new LuaProxy(scene_));
		luaProxy_->runFile(fileName.toAscii());
	}
}

void MainWindow::closeScene()
{
	scene_->clear();
	luaProxy_ = std::tr1::shared_ptr<LuaProxy>(new LuaProxy(scene_));
}

void MainWindow::setDrawStyle(int t)
{
	mainGlWidget_->setStyle((GLWidget::DrawStyle)t);
}

}
