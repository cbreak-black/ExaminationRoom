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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>


namespace Examination
{
	class Scene;
	class GLWidget;
	class LuaProxy;

/**
The main window. It contains only the GLWidget. It also creates most of the other
objects of the program. A seperate application class was found unnecessary. So far.
 
 \author Gerhard Roethlin
*/
class MainWindow : public QWidget
{
    Q_OBJECT

public:
	/**
	Creates the MainWindow and the environment.
	*/
    MainWindow();
	
	/**
	Destroys the the MainWindow and the environment.
	*/
	~MainWindow();

private slots:

private:
    GLWidget * mainGlWidget_;

	Scene * scene_;

	LuaProxy * luaProxy_;
};

}

#endif
