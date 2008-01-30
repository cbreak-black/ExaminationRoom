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

	scene_ = new Scene();
	mainGlWidget_->setScene(scene_);

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
	delete mainGlWidget_;
}

void MainWindow::onTimeout()
{
	luaProxy_->onUpdate();
	mainGlWidget_->repaint(); // update() for deferred updates
}

}
