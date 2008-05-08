/*
 *  parameterdepthbuffer.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 08.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PARAMETERDEPTHBUFFER_H
#define PARAMETERDEPTHBUFFER_H

#include "parameterobject.h"

namespace Examination
{

class ParameterDepthBuffer : public ParameterObject
{
	Q_OBJECT

public:
	ParameterDepthBuffer(std::tr1::shared_ptr<Object> object);

protected:
	/**
	Reloads the data of the associated object.
	 \see ParameterObject::reloadData()
	 */
	virtual void reloadData();

public slots: // From the UI
	void depthStateChanged(int state);

private:
	QCheckBox * checkboxDepth_;
};

}

#endif
