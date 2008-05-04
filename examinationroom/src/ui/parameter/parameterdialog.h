/*
 *  parameterdialog.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 04.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <QWidget>

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

private:
	QLayout * layout_;
};


}
