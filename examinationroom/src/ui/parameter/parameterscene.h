/*
 *  parameterscene.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 26.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "parameterobject.h"

#include <memory>

namespace Examination
{
	class Scene;

class ParameterScene : public ParameterObject
{
	Q_OBJECT

public:
	ParameterScene(std::tr1::shared_ptr<Scene> scene);

public slots:
	void showCameraDialog();

public:
	std::tr1::shared_ptr<Scene> scene() const;
};

}
