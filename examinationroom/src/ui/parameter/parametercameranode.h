/*
 *  parametercameranode.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 26.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterobject.h"

namespace Examination
{

class ParameterCameraNode : public ParameterObject
{
	Q_OBJECT

public:
	ParameterCameraNode(std::tr1::shared_ptr<Object> object);

public slots:
	void showCameraDialog();
};

}
