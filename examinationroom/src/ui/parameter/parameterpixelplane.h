/*
 *  parameterpixelplane.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 07.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PARAMETERPIXELPLANE_H
#define PARAMETERPIXELPLANE_H

#include "parameterrectangle.h"

namespace Examination
{
	
class ParameterPixelplane : public ParameterObject
{
	Q_OBJECT

public:
	ParameterPixelplane(std::tr1::shared_ptr<Object> object);
	
protected:
	/**
	Reloads the data of the associated object.
	 \see ParameterObject::reloadData()
	*/
	virtual void reloadData();

public slots: // From the UI
	void sizeEdited();
	void zoomEdited();
	void autoscaleStateChanged(int state);

private:
	QLineEdit * lineSizeX_;
	QLineEdit * lineSizeY_;
	QLineEdit * lineZoomX_;
	QLineEdit * lineZoomY_;
	QCheckBox * checkboxAutoscale_;
};

}

#endif
