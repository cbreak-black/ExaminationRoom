/*
 *  parametertexture.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 27.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARAMETERTEXTURE_H
#define PARAMETERTEXTURE_H

#include "parameterabstracttexture.h"

class QLineEdit;

namespace Examination
{

class ParameterTexture : public ParameterAbstractTexture
{
	Q_OBJECT

public:
	ParameterTexture(std::tr1::shared_ptr<AbstractTexture> texture);

public:
	virtual void reloadData();

public slots:
	void pathEdited();
	void openTexture();

private:
	QLineEdit * linePath_;
};

}

#endif
