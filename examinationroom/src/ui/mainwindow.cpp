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
#include "scene.h"

#include "objects/object.h"

#include <iostream>
#include <fstream>

namespace Examination
{

MainWindow::MainWindow()
{
	fullScreen_ = false;

	// Set Title and window properties
	setWindowTitle(tr("Examinationroom"));
	QMainWindow::setDockOptions(AnimatedDocks | AllowNestedDocks | AllowTabbedDocks);

	// Create GL Widgets
	QGLFormat glFormat = QGLFormat::defaultFormat();
	// Stereo Buffering seems to disable other things, like double buffering
	//glFormat.setStereo(true);
	QGLFormat::setDefaultFormat(glFormat);

	mainGlWidget_ = new GLWidget(this);
	fsGlWidget_ = new GLWidget(0, mainGlWidget_);
	mainGlWidget_->setStyle(GLWidget::single);
	fsGlWidget_->setStyle(GLWidget::single);

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

	// Add display menu
	QAction * action;
	menu = menuBar()->addMenu(tr("&Output Mode"));

	action = menu->addAction(tr("&Fullscreen"),
					this, SLOT(toggleFullscreen()),
					QKeySequence(Qt::Key_F | Qt::CTRL));
	fsGlWidget_->addAction(action);

	menu->addSeparator();

	action = menu->addAction(tr("&Anaglyph"));
	action->setShortcut(QKeySequence(Qt::Key_1 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::anaglyph);
	fsGlWidget_->addAction(action);

	action = menu->addAction(tr("&Side by Side"));
	action->setShortcut(QKeySequence(Qt::Key_2 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::sidebyside);
	fsGlWidget_->addAction(action);

	action = menu->addAction(tr("&Line interlacing (Experimental)"));
	action->setShortcut(QKeySequence(Qt::Key_3 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::line);
	fsGlWidget_->addAction(action);

	action = menu->addAction(tr("Quad Buffering (Experimental)"));
	action->setShortcut(QKeySequence(Qt::Key_4 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::quad);
	fsGlWidget_->addAction(action);

	action = menu->addAction(tr("Matrix Anaglyph (Experimental)"));
	action->setShortcut(QKeySequence(Qt::Key_5 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::matrix);
	fsGlWidget_->addAction(action);

	menu->addSeparator();

	action = menu->addAction(tr("Si&ngle"));
	action->setShortcut(QKeySequence(Qt::Key_0 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::single);
	fsGlWidget_->addAction(action);

	connect(signalMapper_, SIGNAL(mapped(int)), this, SLOT(setDrawStyle(int)));

	// Object Creation Menu and signal mapper
	objectMenu_ = menuBar()->addMenu(tr("&Create Object"));
	objectMapper_ = new QSignalMapper(this);
	connect(objectMapper_, SIGNAL(mapped(int)), this, SLOT(onObjectCreate(int)));

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
	delete fsGlWidget_;
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
		fsGlWidget_->update();
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
	program_->onKeyUp(event->key());
	onUpdate();
}

void MainWindow::onTimeout()
{
	program_->onUpdate();
	onUpdate();
}

void MainWindow::onUpdate()
{
	if (fullScreen_)
		fsGlWidget_->update();
	else
		mainGlWidget_->update(); // update() for deferred updates
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
	fsGlWidget_->setStyle((GLWidget::DrawStyle)t);
}

void MainWindow::toggleFullscreen()
{
	if (fullScreen_)
	{
		fullScreen_ = false;
		fsGlWidget_->hide();
		releaseKeyboard();
//		mainGlWidget_->show();
	}
	else
	{
		fullScreen_ = true;
		grabKeyboard();
//		mainGlWidget_->hide();
		fsGlWidget_->showFullScreen();
		int numScreens = QApplication::desktop()->numScreens();
		if (numScreens >= 2)
		{
			// Two Screens used by output widget
			QRect s2 = QApplication::desktop()->screenGeometry(0);
			s2 = s2.united(QApplication::desktop()->screenGeometry(1));
			fsGlWidget_->setGeometry(s2);
		}
	}
}

void MainWindow::newScene()
{
	setProgram(Program::create());
}

void MainWindow::setProgram(std::tr1::shared_ptr<Program> program)
{
	program_ = program;
	mainGlWidget_->setScene(program_->scene());
	fsGlWidget_->setScene(program_->scene());
	dockDesign_->setProgram(program);
	// Recreate menu
	objectMenu_->clear(); // Should also destroy all mappings
	const std::vector<ObjectFactoryPtr> & factories = program->factories();
	QAction * action;
	for (unsigned int i = 0; i < factories.size(); i++)
	{
		action = objectMenu_->addAction(QString::fromStdString(factories[i]->name()), objectMapper_, SLOT(map()));
		objectMapper_->setMapping(action, i);
	}
}

void MainWindow::onObjectCreate(int id)
{
	// Create a new object
	std::tr1::shared_ptr<Object> o = program_->factories()[id]->create();
	// And find a place to put it
	std::tr1::shared_ptr<Object> s = dockDesign_->selectedObject();

	if (s)
	{
		std::tr1::shared_ptr<Container> c = std::tr1::dynamic_pointer_cast<Container>(s);
		std::tr1::shared_ptr<Container> p = s->parent();
		if (c)
		{
			// A container selected, insert into it
			c->addObject(o);
		}
		else if (p)
		{
			// An object selected, insert into its parent
			p->addObject(o);
		}
		else
		{
			// Selected object has no parent, should not happen!
			program_->scene()->addObject(o);
		}
	}
	else
	{
		// Nothing useful selected, insert as root
		program_->scene()->addObject(o);
	}
}

}
