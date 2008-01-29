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
}


MainWindow::~MainWindow()
{
	delete scene_;
}

}
