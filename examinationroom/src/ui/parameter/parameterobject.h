/*
 *  parameterobject.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 04.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterdialog.h"

#include <memory>

class QLineEdit;
class QLabel;

namespace Examination
{
	class Object;

class ParameterObject : public Parameterdialog
{
public:
	ParameterObject(std::tr1::shared_ptr<Object> object);
	~ParameterObject();

private:
	std::tr1::shared_ptr<Object> object_;

	QLineEdit * lineName_;
	QLineEdit * linePosX_;
	QLineEdit * linePosY_;
	QLineEdit * linePosZ_;
	QLabel * colorLabel_;
};


}
