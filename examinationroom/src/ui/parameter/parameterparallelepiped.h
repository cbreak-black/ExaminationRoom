/*
 *  parameterparallelepiped.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 07.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PARAMETERPARALLELEPIPED_H
#define PARAMETERPARALLELEPIPED_H

#include "parameterrectangle.h"

namespace Examination
{

class ParameterParallelepiped : public ParameterRectangle
{
	Q_OBJECT

public:
	ParameterParallelepiped(std::tr1::shared_ptr<Object> object);

protected:
	/**
	Reloads the data of the associated object.
	 \see ParameterObject::reloadData()
	*/
	virtual void reloadData();

public slots: // From the UI
	void dirCEdited();

private:
	QLineEdit * lineDirCX_;
	QLineEdit * lineDirCY_;
	QLineEdit * lineDirCZ_;
};

}

#endif
