/*
 *  parameterobject.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 04.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARAMETEROBJECT_H
#define PARAMETEROBJECT_H

#include "parameterdialog.h"

#include <memory>

class QLineEdit;
class QLabel;
class QCheckBox;
class QComboBox;
class QPushButton;

namespace Examination
{
	class Object;

class ParameterObject : public Parameterdialog
{
	Q_OBJECT

public:
	ParameterObject(std::tr1::shared_ptr<Object> object);
	~ParameterObject();

public: // Callbacks
	void objectDidChange();

protected:
	/**
	Reload the data from the data source (the object).
	This method can be reimplemented by subclasses to reload their own data.
	Do not forget to call the super class implementaton.
	*/
	virtual void reloadData();

	/**
	Returns a shared pointer to the associated data source object.
	If the object does not exist anymore (the internal weak pointer is invalid)
	an invalid shared pointer is returned.
	 \return shared_ptr to the data source object
	*/
	std::tr1::shared_ptr<Object> object() const;

public slots: // From the UI
	void nameEdited();
	void posXEdited();
	void posYEdited();
	void posZEdited();
	void priorityEdited();
	void wireframeStateChanged(int state);
	void setColor();
	void editTexture();
	void setTexture(int);

private:
	std::tr1::weak_ptr<Object> object_;

	QLineEdit * lineName_;
	QLineEdit * linePosX_;
	QLineEdit * linePosY_;
	QLineEdit * linePosZ_;
	QLineEdit * linePriority_;
	QLabel * colorLabel_;
	QCheckBox * checkboxWireframe_;
	QComboBox * comboTex_;
	QPushButton * buttonEditTex_;
};


}

#endif
