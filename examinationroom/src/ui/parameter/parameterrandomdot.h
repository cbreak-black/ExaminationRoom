/*
 *  parameterrandomdot.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 27.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARAMETERRANDOMDOT_H
#define PARAMETERRANDOMDOT_H

#include "parameterstereogram.h"

class QLineEdit;

namespace Examination
{

class ParameterRandomDot : public ParameterStereogram
{
	Q_OBJECT

public:
	ParameterRandomDot(std::tr1::shared_ptr<AbstractTexture> texture);

public:
	virtual void reloadData();

public slots:
	void maxColorsEdited();
	void excColorsEdited();

private:
	QLineEdit * lineMaxColors_;
	QLineEdit * lineExcColors_;
};

}

#endif