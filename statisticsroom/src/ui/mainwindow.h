/*
 *  mainwindow.h
 *  StatisticsRoom
 *
 *  Created by CBreak on 29.02.08
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Statistics
{

class MainWindow : public QWidget
{
public:
	/**
	Creates the MainWindow and the environment.
	*/
    MainWindow();
	
	/**
	Destroys the the MainWindow and the environment.
	*/
	~MainWindow();
};

}

#endif
