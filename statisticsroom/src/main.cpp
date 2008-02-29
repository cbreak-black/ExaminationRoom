/*******************************************************************************

Statistics Room is a flexible log analyser for Examination Room

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

int main (int argc, char** argv)
{
    QApplication app(argc, argv);

	Statistics::MainWindow mw;
	mw.show();

    return app.exec();
}
