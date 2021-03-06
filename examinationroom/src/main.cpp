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

#include <cstdlib>
#include <ctime>
#include <locale.h>

int main (int argc, char** argv)
{
	srand (time(0));

    QApplication app(argc, argv);

	// Set the locale back to "C", the creator of QApplication just changed it
	setlocale(LC_NUMERIC, "C");
	
	Examination::MainWindow mw;
	mw.show();
	
    return app.exec();
}
