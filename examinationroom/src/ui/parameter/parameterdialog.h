/*
 *  parameterdialog.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 04.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARAMETERDIALOG_H
#define PARAMETERDIALOG_H

#include <QWidget>
#include <string>

class QLayout;

namespace Examination
{

class Parameterdialog : public QWidget
{
public:
	Parameterdialog(QWidget * parent = 0);
	~Parameterdialog();

public:
	/**
	Returns the preferred size of parameter dialogs
	 \return	the preferred size of this dialog
	*/
	virtual QSize sizeHint () const;

protected:
	/**
	Adds a widget (usually a QGroupBox) to the layout.
	 \param w	A pointer to a QWidget instance
	*/
	void addWidget(QWidget * w);

protected:
	/**
	Helper method.
	Displays a file-open dialog, and returns the file name as path
	relative to the current directory.
	 \return A path of an opened file
	*/
	std::string openFileRelative(const char * caption = 0,
								 const char * filter = 0);

private:
	QLayout * layout_;
};


}

#endif
