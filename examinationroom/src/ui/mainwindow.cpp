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
	
	// Floor
	std::string path = std::string("../../res/checkerboard.png");
	Rectangle * r = new Rectangle();
	r->setDirA(Tool::Point(3,0,0));
	r->setDirB(Tool::Point(0,0,6));
	r->setPosition(Tool::Point(0, -3, -3));
	r->setTexture(new Texture(path));
	r->setTexCoords(0,0, 0,8, 4,0, 4,8);
	scene_->addOject(r);
	
	// Ceil
	r = new Rectangle();
	r->setDirA(Tool::Point(3,0,0));
	r->setDirB(Tool::Point(0,0,6));
	r->setPosition(Tool::Point(0, 3, -3));
	r->setTexture(new Texture(path));
	r->setTexCoords(0,0, 0,8, 4,0, 4,8);
	scene_->addOject(r);

	// Plane
	path = std::string("../../res/triangle.png");
	r = new Rectangle();
	r->setDirA(Tool::Point(1,0,0));
	r->setDirB(Tool::Point(0,1,0));
	r->setPosition(Tool::Point(-2, -2, 0));
	r->setTexture(new Stereogram(new Texture(path)));
	scene_->addOject(r);
}


MainWindow::~MainWindow()
{
	delete scene_;
}

}
