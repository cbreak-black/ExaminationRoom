/*
 *  parameterstereogram.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 27.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARAMETERSTEREOGRAM_H
#define PARAMETERSTEREOGRAM_H

#include "parameterabstracttexture.h"

class QLineEdit;
class QComboBox;

namespace Examination
{

class ParameterStereogram : public ParameterAbstractTexture
{
	Q_OBJECT

public:
	ParameterStereogram(std::tr1::shared_ptr<AbstractTexture> texture);

public:
	virtual void reloadData();

public slots:
	void pathDepthEdited();
	void pathLeftEdited();
	void pathRightEdited();
	void offsetEdited();
	void styleActivated(int);

private:
	QLineEdit * linePathDepth_;
	QLineEdit * linePathLeft_;
	QLineEdit * linePathRight_;
	QLineEdit * lineOffset_;
	QComboBox * comboStyle_;
};

}

#endif