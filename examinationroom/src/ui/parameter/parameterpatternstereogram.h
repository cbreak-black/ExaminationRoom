/*
 *  parameterpatternstereogram.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 27.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARAMETERPATTERNSTEREOGRAM_H
#define PARAMETERPATTERNSTEREOGRAM_H

#include "parameterstereogram.h"

class QLineEdit;
class QPushButton;

namespace Examination
{

class ParameterPatternStereogram : public ParameterStereogram
{
	Q_OBJECT

public:
	ParameterPatternStereogram(std::tr1::shared_ptr<AbstractTexture> texture);

public:
	virtual void reloadData();

public slots:
	void pathBGEdited();
	void pathFGEdited();
	void openBG();
	void openFG();

private:
	QLineEdit * linePathBG_;
	QLineEdit * linePathFG_;
	QPushButton * pbBG_;
	QPushButton * pbFG_;
};

}

#endif