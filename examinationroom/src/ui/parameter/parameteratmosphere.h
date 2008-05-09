/*
 *  parameteratmosphere.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 08.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PARAMETERATMOSPHERE_H
#define PARAMETERATMOSPHERE_H

#include "parameterobject.h"

class QComboBox;

namespace Examination
{

class ParameterAtmosphere : public ParameterObject
{
	Q_OBJECT

public:
	ParameterAtmosphere(std::tr1::shared_ptr<Object> object);

protected:
	/**
	Reloads the data of the associated object.
	 \see ParameterObject::reloadData()
	*/
	virtual void reloadData();

public slots: // From the UI
	void densityEdited();
	void startEdited();
	void endEdited();
	void modeActivated(int mode);

private:
	QLineEdit * lineDensity_;
	QLineEdit * lineStart_;
	QLineEdit * lineEnd_;
	QComboBox * pulldownMode_;
};

}

#endif
