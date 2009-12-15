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

#include <iostream>
#include <iomanip>

namespace Examination
{

unsigned int ErrorTool::getErrors(std::string fName, std::string oName)
{
	unsigned int n = 0;
	GLenum err = glGetError();
	GLenum errLast = 0;
	while (err != GL_NO_ERROR)
	{
		n++;
		const GLubyte * str = gluErrorString(err);
		std::cerr << "OpenGL Error: [" << fName << ", " << oName << "] ";
		std::cerr << "0x" << std::setfill('0') << std::setw(sizeof(GLubyte)) << std::hex
			<< err << std::setfill(' ');
		if (str)
			std::cerr << " " << str;
		if (errLast == err)
		{
			std::cerr << " (repeated)" << std::endl;
			return n;
		}
		std::cerr << std::endl;
		errLast = err;
		err = glGetError();
	}
	return n;
}

void ErrorTool::logError(std::string fName, std::string error)
{
	std::cerr << "General Error: [" << fName << "] " << error << std::endl;
}

}
