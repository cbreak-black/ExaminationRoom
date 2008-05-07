/*
 *  parametersphere.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 07.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PARAMETERSPHERE_H
#define PARAMETERSPHERE_H

#include "parameterobject.h"

namespace Examination
{

class ParameterSphere : public ParameterObject
{
	Q_OBJECT

public:
	ParameterSphere(std::tr1::shared_ptr<Object> object);

protected:
	/**
	Reloads the data of the associated object.
	 \see ParameterObject::reloadData()
	*/
	virtual void reloadData();

public slots: // From the UI
	void radiusEdited();
	void slicesEdited();
	void stacksEdited();

private:
	QLineEdit * lineRadius_;
	QLineEdit * lineSlices_;
	QLineEdit * lineStacks_;
};

}

#endif
