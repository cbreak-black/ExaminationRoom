/*
 *  parameterlightnode.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 08.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PARAMETERLIGHTNODE_H
#define PARAMETERLIGHTNODE_H

#include "parameterobject.h"

namespace Examination
{

class ParameterLightNode : public ParameterObject
{
	Q_OBJECT

public:
	ParameterLightNode(std::tr1::shared_ptr<Object> object);

protected:
	/**
	Reloads the data of the associated object.
	 \see ParameterObject::reloadData()
	*/
	virtual void reloadData();

public slots: // From the UI
	void setAmbient();

private:
	QLabel * ambientLabel_;
};

}

#endif
