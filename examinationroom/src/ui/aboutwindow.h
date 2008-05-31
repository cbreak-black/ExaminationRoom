/*
 *  aboutwindow.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 30.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>

namespace Examination
{

/**
A small helper class containing the about window.
It's only instanciated once, and just a simple container
for data.
*/
class AboutWindow : public QDialog
{
public:
	AboutWindow(QWidget * parent = 0, Qt::WindowFlags f = 0);
};

}

#endif
