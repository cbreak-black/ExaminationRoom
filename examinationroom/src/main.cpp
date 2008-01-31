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

#include <QApplication>

#include "ui/mainwindow.h"

using namespace Examination;

int main (int argc, char** argv)
{
    QApplication app(argc, argv);
	
	MainWindow mw;
	mw.showFullScreen();
	
    return app.exec();
}
