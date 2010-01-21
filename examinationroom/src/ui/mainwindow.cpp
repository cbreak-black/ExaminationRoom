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
#include "codewidget.h"
#include "consolewidget.h"
#include "aboutwindow.h"

#include "program.h"
#include "scene.h"

#include "objects/object.h"

#include "glerrortool.h"
#include "logtool.h"

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

	// Add Dock Widgets
	dockDesign_ = new DesignWidget("Design", this);
	dockCode_ = new CodeWidget("Code", this);
	dockConsole_ = new ConsoleWidget("Console", this);
	dockDesign_->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	dockCode_->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	dockConsole_->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	addDockWidget(Qt::RightDockWidgetArea, dockDesign_);
	addDockWidget(Qt::RightDockWidgetArea, dockCode_);
	addDockWidget(Qt::RightDockWidgetArea, dockConsole_);
	tabifyDockWidget(dockDesign_, dockCode_);
	tabifyDockWidget(dockDesign_, dockConsole_);

	// Set up logging early
	LogTool::initLogFile();
	LogTool::setConsoleWidget(dockConsole_);

	// File dialogs
#ifdef Q_WS_MACX
	Qt::WindowFlags dialogFlags = Qt::Sheet;
	QString defaultPath = QString("res/");
#else
	Qt::WindowFlags dialogFlags = Qt::Dialog;
	QString defaultPath = QString("res/");
#endif
	loadDialog_ = new QFileDialog(this, dialogFlags);
	loadDialog_->setFilter("Lua Scene File (*.lua)");
	loadDialog_->setWindowTitle("Open Lua Scene...");
	loadDialog_->setDirectory(defaultPath);
	storeDialog_ = new QFileDialog(this, dialogFlags);
	storeDialog_->setAcceptMode(QFileDialog::AcceptSave);
	storeDialog_->setFilter("Lua Scene File (*.lua)");
	storeDialog_->setWindowTitle("Save Lua Scene...");
	storeDialog_->setDirectory(defaultPath);
	storeDialog_->setDefaultSuffix("lua");
	connect(loadDialog_, SIGNAL(fileSelected(const QString &)), this, SLOT(loadLuaFile(const QString &)));
	connect(storeDialog_, SIGNAL(fileSelected(const QString &)), this, SLOT(storeLuaFile(const QString &)));

	// Create GL Widgets
	QGLFormat glFormat = QGLFormat::defaultFormat();
	// Stereo Buffering seems to work in SnowLeopard...
	// but is incompatible with other renderers and might cause problems.
	// enable at your own risk
	//glFormat.setStereo(true);
	glFormat.setSwapInterval(1); // Enable VSync on platforms that support it
	QGLFormat::setDefaultFormat(glFormat);

	// Checking for errors here leads to a crash on OS X
	// Probably because OpenGL was not initialized yet?
	//ErrorTool::getErrors("MainWindow::MainWindow:1");
	mainGlWidget_ = new GLWidget(this);
	fsGlWidget_ = new GLWidget(0, mainGlWidget_);
	fsGlWidget_->setCursor(Qt::BlankCursor);
	mainGlWidget_->setStyle(GLWidget::single);
	fsGlWidget_->setStyle(GLWidget::single);
	mainGlWidget_->makeCurrent();
	GlErrorTool::getErrors("MainWindow::MainWindow:2");
	GlErrorTool::logVersionStrings();

	aboutWindow_ = 0;

	// Add menu
	QMenu *menu = menuBar()->addMenu(tr("&File"));
	menu->addAction(tr("&About Examinationroom..."),
					this, SLOT(showAbout()));
	menu->addAction(tr("&Open Scene..."),
					loadDialog_, SLOT(open()),
					QKeySequence(QKeySequence::Open));
	menu->addAction(tr("Close Scene"),
					this, SLOT(newScene()),
					QKeySequence(QKeySequence::Close));
	menu->addAction(tr("&Save Copy As..."),
					storeDialog_, SLOT(open()),
					QKeySequence(QKeySequence::Save));
	menu->addAction(tr("&Revert Scene"),
					this, SLOT(revert()));
	menu->addSeparator();
	menu->addAction(tr("&Quit"), this, SLOT(close()),
					QKeySequence(tr("ctrl-Q")));

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

	action = menu->addAction(tr("&Shader (Line interlacing)"));
	action->setShortcut(QKeySequence(Qt::Key_6 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::shaderLine);
	fsGlWidget_->addAction(action);

	action = menu->addAction(tr("&Shader (Mayan)"));
	action->setShortcut(QKeySequence(Qt::Key_7 | Qt::CTRL));
	connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
	signalMapper_->setMapping(action, GLWidget::shaderMayan);
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

#ifdef Q_WS_MACX
	// Make menu bar the default menu bar
	menuBar()->setParent(0);
#endif

	// Set up layout
	setFocusPolicy(Qt::StrongFocus);
	setCentralWidget(mainGlWidget_);

	// Create Scene
	setProgram(Program::create());

	// Set up redraw timer
	timer_ = new QTimer(this);
	connect(timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));
	timer_->start(15); // ~60 fps
}


MainWindow::~MainWindow()
{
	LogTool::setConsoleWidget(NULL);
	LogTool::closeLogFile();
	// All child widgets are deallocated by QT
	delete fsGlWidget_;
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

void MainWindow::loadLuaFile(const QString & fileName)
{
	if (!fileName.isNull())
	{
		setProgram(Program::create());
		program_->loadLua(fileName.toStdString(), true);
	}
}

void MainWindow::storeLuaFile(const QString & fileName)
{
	if (!fileName.isNull())
	{
		std::ofstream of;
		of.open(fileName.toAscii(), std::ios_base::out);
		program_->toLua(of);
	}
}

void MainWindow::revert()
{
	// Don't use Program::revert(), since we want to register our
	// event handlers before loading the root file
	std::string rootFile = program_->rootFile();
	setProgram(Program::create());
	program_->loadLua(rootFile, true);
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
		fsGlWidget_->showNormal();
		fsGlWidget_->hide();
		releaseKeyboard();
		mainGlWidget_->makeCurrent();
//		mainGlWidget_->show();
	}
	else
	{
		fullScreen_ = true;
		grabKeyboard();
//		mainGlWidget_->hide();
		fsGlWidget_->makeCurrent();
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
	mainGlWidget_->setProgram(program_);
	fsGlWidget_->setProgram(program_);
	dockDesign_->setProgram(program);
	dockCode_->setProgram(program);
	dockConsole_->setProgram(program);
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

void MainWindow::showAbout()
{
	if (!aboutWindow_)
		aboutWindow_ = new AboutWindow();
	aboutWindow_->show();
}

}
