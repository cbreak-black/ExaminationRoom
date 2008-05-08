/*
 *  parametermesh.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 08.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PARAMETERMESH_H
#define PARAMETERMESH_H

#include "parameterobject.h"

namespace Examination
{

class ParameterMesh : public ParameterObject
{
	Q_OBJECT

public:
	ParameterMesh(std::tr1::shared_ptr<Object> object);

protected:
	/**
	Reloads the data of the associated object.
	 \see ParameterObject::reloadData()
	*/
	virtual void reloadData();

public slots: // From the UI
	void pathEdited();
	void scaleEdited();

private:
	QLineEdit * linePath_;
	QLineEdit * lineScale_;
};

}

#endif
