/*
 *  parameterabstracttexture.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 26.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARAMETERABSTRACTTEXTURE_H
#define PARAMETERABSTRACTTEXTURE_H

#include "parameterdialog.h"

#include <memory>

class QLineEdit;

namespace Examination
{
	class AbstractTexture;

class ParameterAbstractTexture : public Parameterdialog
{
	Q_OBJECT

public:
	/**
	Creates a new dialog.
	*/
	ParameterAbstractTexture(std::tr1::shared_ptr<AbstractTexture> texture);

	virtual ~ParameterAbstractTexture();

public slots:
	void sizeReset();
	void sizeEdited();
	void zoomEdited();

public:
	virtual void reloadData();
	std::tr1::shared_ptr<AbstractTexture> texture() const;

private:
	std::tr1::weak_ptr<AbstractTexture> texture_;

	QLineEdit * lineSizeX_;
	QLineEdit * lineSizeY_;
	QLineEdit * lineZoomX_;
	QLineEdit * lineZoomY_;
};

}

#endif
