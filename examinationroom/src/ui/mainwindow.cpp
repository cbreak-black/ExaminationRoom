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
	
	lGlWidget_ = new GLWidget(0, mainGlWidget_);
	rGlWidget_ = new GLWidget(0, mainGlWidget_);

	scene_ = new Scene();
	mainGlWidget_->setScene(scene_);
	lGlWidget_->setScene(scene_);
	rGlWidget_->setScene(scene_);
	lGlWidget_->setSide(left);
	rGlWidget_->setSide(right);
	
	lGlWidget_->show();
	rGlWidget_->show();

	luaProxy_ = new LuaProxy(scene_);
	luaProxy_->runFile("res/scene.lua");

	timer_ = new QTimer(this);
	connect(timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));
	timer_->start(33); // 30 fps
}


MainWindow::~MainWindow()
{
	delete timer_;
	delete luaProxy_;
	delete scene_;
	delete rGlWidget_;
	delete lGlWidget_;
	delete mainGlWidget_;
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	luaProxy_->onKeyDown(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
	luaProxy_->onKeyUp(event->key());
}

void MainWindow::onTimeout()
{
	luaProxy_->onUpdate();
	mainGlWidget_->repaint(); // update() for deferred updates
	lGlWidget_->repaint();
	rGlWidget_->repaint();
}

}
