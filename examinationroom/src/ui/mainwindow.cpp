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

#include "rectangle.h"
#include "texture.h"
#include "stereogram.h"

#include "luaproxy.h"

namespace Examination
{

MainWindow::MainWindow()
{
    QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);
	
	mainGlWidget_ = new GLWidget(this);
	mainLayout->addWidget(mainGlWidget_, 0, 0);

	setLayout(mainLayout);

	setFocusPolicy(Qt::StrongFocus);
	
	outGlWidget_ = new GLWidget(0, mainGlWidget_);

	scene_ = new Scene();
	mainGlWidget_->setScene(scene_);
	outGlWidget_->setScene(scene_);
	
	int numScreens = QApplication::desktop()->numScreens();

	if (numScreens >= 3)
	{
		// Main Screen used by main widget
		// Other two by output widget
		this->showFullScreen();
		outGlWidget_->showFullScreen();
		this->setGeometry(QApplication::desktop()->screenGeometry(0));
		QRect s2 = QApplication::desktop()->screenGeometry(1);
		s2 = s2.united(QApplication::desktop()->screenGeometry(2));
		outGlWidget_->setGeometry(s2);
		mainGlWidget_->setStyle(GLWidget::anaglyph);
		outGlWidget_->setStyle(GLWidget::sidebyside);
	}
	if (numScreens == 2)
	{
		// Two Screens used by main widget
		this->showFullScreen();
		QRect s2 = QApplication::desktop()->screenGeometry(0);
		s2 = s2.united(QApplication::desktop()->screenGeometry(1));
		this->setGeometry(s2);
		mainGlWidget_->setStyle(GLWidget::sidebyside);
	}
	if (numScreens == 1)
	{
		this->showFullScreen();
		this->setGeometry(QApplication::desktop()->screenGeometry(0));
		mainGlWidget_->setStyle(GLWidget::anaglyph);
	}

	luaProxy_ = new LuaProxy(scene_);
	luaProxy_->runFile("res/scene.lua");

	timer_ = new QTimer(this);
	connect(timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));
	//timer_->start(33); // 30 fps
	timer_->start(100); // 30 fps
}


MainWindow::~MainWindow()
{
	delete timer_;
	delete luaProxy_;
	delete scene_;
	delete outGlWidget_;
	delete mainGlWidget_;
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	luaProxy_->onKeyDown(event->key());
	mainGlWidget_->update(); // update() for deferred updates
	outGlWidget_->update();
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
