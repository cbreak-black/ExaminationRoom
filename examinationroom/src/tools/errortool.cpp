/*
 *  errortool.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 01.12.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "errortool.h"

#include <qgl.h>
#include <sstream>
#include <iomanip>

#include "logtool.h"

namespace Examination
{

unsigned int ErrorTool::getErrors(const std::string & fName, const std::string & oName)
{
	unsigned int n = 0;
	GLenum err = glGetError();
	GLenum errLast = 0;
	while (err != GL_NO_ERROR)
	{
		n++;
		const GLubyte * str = gluErrorString(err);
		std::stringstream ss(std::ios_base::out | std::ios_base::app);
		ss << "0x" << std::setfill('0') << std::setw(sizeof(GLubyte)*2) << std::hex << err;
		if (str)
			ss << " " << str;
		if (errLast == err)
		{
			ss << " (repeated)";
			LogTool::logError("OPENGL", fName+", "+oName, ss.str());
			return n;
		}
		LogTool::logError("OPENGL", fName+", "+oName, ss.str());
		errLast = err;
		err = glGetError();
	}
	return n;
}

}
