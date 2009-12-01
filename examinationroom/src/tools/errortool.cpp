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
	while (err != GL_NO_ERROR)
	{
		n++;
		const GLubyte * str = gluErrorString(err);
		std::cerr << "OpenGL Error: [" << fName << ", " << oName << "] ";
		if (str)
			std::cerr << str << std::endl;
		else
			std::cerr << " 0x" << std::setfill('0') << std::setw(sizeof(GLubyte)) << std::hex
			<< err << std::setfill(' ') << std::endl;
		err = glGetError();
	}
	return n;
}

void ErrorTool::logError(std::string fName, std::string error)
{
	std::cerr << "General Error: [" << fName << "] " << error << std::endl;
}

}
