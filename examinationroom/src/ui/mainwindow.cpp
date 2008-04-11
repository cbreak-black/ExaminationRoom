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

#include "luaproxy.h"

#include <qgl.h>

namespace Examination
{

MainWindow::MainWindow()
{
    QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);
	
	QGLFormat glFormat;
	glFormat.setStereo(true);

	mainGlWidget_ = new GLWidget(glFormat, this);
	mainLayout->addWidget(mainGlWidget_, 0, 0);

	setLayout(mainLayout);

	setFocusPolicy(Qt::StrongFocus);
	
	outGlWidget_ = new GLWidget(glFormat, 0, mainGlWidget_);

	scene_ = std::tr1::shared_ptr<Scene>(new Scene());
	mainGlWidget_->setScene(scene_);
	outGlWidget_->setScene(scene_);
	
	int numScreens = QApplication::desktop()->numScreens();

	if (numScreens >= 3)
	{
		// Main Screen used by main widget
		// Other two by output widget
		mainGlWidget_->setStyle(GLWidget::anaglyph);
		outGlWidget_->setStyle(GLWidget::sidebyside);
		this->show();
		outGlWidget_->showFullScreen();
		this->setGeometry(QApplication::desktop()->screenGeometry(0));
		QRect s2 = QApplication::desktop()->screenGeometry(1);
		s2 = s2.united(QApplication::desktop()->screenGeometry(2));
		outGlWidget_->setGeometry(s2);
	}
	if (numScreens == 2)
	{
		// Two Screens used by main widget
		mainGlWidget_->setStyle(GLWidget::sidebyside);
		this->showFullScreen();
		QRect s2 = QApplication::desktop()->screenGeometry(0);
		s2 = s2.united(QApplication::desktop()->screenGeometry(1));
		this->setGeometry(s2);
	}
	if (numScreens == 1)
	{
		mainGlWidget_->setStyle(GLWidget::matrix);
		this->showFullScreen();
		//this->setGeometry(QApplication::desktop()->screenGeometry(0));
	}

	luaProxy_ = std::tr1::shared_ptr<LuaProxy>(new LuaProxy(scene_));
	timer_ = new QTimer(this);
	connect(timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));
	timer_->start(33); // 30 fps
	
	QString fileName = QFileDialog::getOpenFileName(this, "Open Log File",
		"res/scene.lua",
		"Lua Scene File (*.lua)");
	if (!fileName.isNull())
	{
		luaProxy_->runFile(fileName.toAscii());
	}
}


MainWindow::~MainWindow()
{
	delete timer_;
	delete outGlWidget_;
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
		outGlWidget_->update();
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
	luaProxy_->onKeyUp(event->key());
	mainGlWidget_->update(); // update() for deferred updates
	outGlWidget_->update();
}

void MainWindow::onTimeout()
{
	luaProxy_->onUpdate();
	mainGlWidget_->update(); // update() for deferred updates
	outGlWidget_->update();
}

}
