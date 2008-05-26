/*
 *  parametercamera.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 26.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterdialog.h"

#include <memory>

class QLineEdit;
class QComboBox;

namespace Examination
{
	class Camera;

class ParameterCamera : public Parameterdialog
{
	Q_OBJECT

private:
	/**
	Creates a ParameterCamera.
	*/
	ParameterCamera();

private:
	/**
	Loads a camera into the camera dialog.
	The passed camera is changed by the dialog.
	 \param cam	The camera that will be changed
	*/
	void loadCamera(std::tr1::shared_ptr<Camera> cam);

	virtual void closeEvent(QCloseEvent * event);

public slots:
	void typeActivated(int i);
	void posEdited();
	void dirEdited();
	void upEdited();
	void sepEdited();
	void fovEdited();
	void ppEdited();

private:
	/**
	Internal camera pointer.
	This is the camera that gets changed and read to initialize values.
	*/
	std::tr1::shared_ptr<Camera> cam_;

	QComboBox * pulldownType_;
	QLineEdit * linePosX_;
	QLineEdit * linePosY_;
	QLineEdit * linePosZ_;
	QLineEdit * lineDirX_;
	QLineEdit * lineDirY_;
	QLineEdit * lineDirZ_;
	QLineEdit * lineUpX_;
	QLineEdit * lineUpY_;
	QLineEdit * lineUpZ_;
	QLineEdit * lineSep_;
	QLineEdit * lineFoV_;
	QLineEdit * linePP_;

public: // Static manager
	/**
	Shows the camera dialog with the passed camera as data source
	and changes it.
	*/
	static void showCameraDialog(std::tr1::shared_ptr<Camera> cam);

private: // Static instance
	/**
	Pointer to the only existing ParameterCamera instance.
	*/
	static std::tr1::shared_ptr<ParameterCamera> cameraDialog_;
};

}
