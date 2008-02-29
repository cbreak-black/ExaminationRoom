/*
 *  mainwindow.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 29.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "mainwindow.h"

#include <QtGui>

namespace Statistics
{

MainWindow::MainWindow()
{
    QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);

	setLayout(mainLayout);
}


MainWindow::~MainWindow()
{
}


}
