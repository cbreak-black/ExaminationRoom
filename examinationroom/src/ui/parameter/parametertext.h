/*
 *  parametertext.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 07.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PARAMETERTEXT_H
#define PARAMETERTEXT_H

#include "parameterobject.h"

namespace Examination
{

class ParameterText : public ParameterObject
{
	Q_OBJECT

public:
	ParameterText(std::tr1::shared_ptr<Object> object);

protected:
	/**
	Reloads the data of the associated object.
	 \see ParameterObject::reloadData()
	*/
	virtual void reloadData();

public slots: // From the UI
	void textEdited();
	void dimEdited();

private:
	QLineEdit * lineText_;
	QLineEdit * lineDimX_;
	QLineEdit * lineDimY_;
};

}

#endif
