/*******************************************************************************

Examination Room is a flexible user evaluation tool

Copyright (C) 2008 Gerhard Roethlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************************/

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
