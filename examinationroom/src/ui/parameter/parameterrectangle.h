/*
 *  parameterrectangle.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 07.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PARAMETERRECTANGLE_H
#define PARAMETERRECTANGLE_H

#include "parameterobject.h"

namespace Examination
{

class ParameterRectangle : public ParameterObject
{
	Q_OBJECT

public:
	ParameterRectangle(std::tr1::shared_ptr<Object> object);

protected:
	/**
	Reloads the data of the associated object.
	 \see ParameterObject::reloadData()
	*/
	virtual void reloadData();

public slots: // From the UI
	void dirAEdited();
	void dirBEdited();
	void subdivisionEdited();
	void texAEdited();
	void texBEdited();
	void texCEdited();
	void texDEdited();

private:
	QLineEdit * lineDirAX_;
	QLineEdit * lineDirAY_;
	QLineEdit * lineDirAZ_;
	QLineEdit * lineDirBX_;
	QLineEdit * lineDirBY_;
	QLineEdit * lineDirBZ_;
	QLineEdit * lineSubdivision_;
	QLineEdit * lineTexAX_;
	QLineEdit * lineTexAY_;
	QLineEdit * lineTexBX_;
	QLineEdit * lineTexBY_;
	QLineEdit * lineTexCX_;
	QLineEdit * lineTexCY_;
	QLineEdit * lineTexDX_;
	QLineEdit * lineTexDY_;
};

}

#endif
