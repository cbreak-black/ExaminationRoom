/*
 *  glerrortool.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 01.12.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "glerrortool.h"

#include <qgl.h>
#include <sstream>
#include <iomanip>

#include "logtool.h"

namespace Examination
{

unsigned int GlErrorTool::getErrors(const std::string & fName, const std::string & oName)
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

void GlErrorTool::logVersionStrings()
{
	// Don't like the reinterpret_cast either, but it was the only way...
	const char * s = NULL;
	s = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	if (s)
		LogTool::logMessage("OpenGL", std::string("GL_VENDOR: ")+s);
	s = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	if (s)
		LogTool::logMessage("OpenGL", std::string("GL_RENDERER: ")+s);
	s = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	if (s)
		LogTool::logMessage("OpenGL", std::string("GL_VERSION: ")+s);
	s = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	if (s)
		LogTool::logMessage("OpenGL", std::string("GL_SHADING_LANGUAGE_VERSION: ")+s);
//	s = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
//	if (s)
//		LogTool::logMessage("OpenGL", std::string("GL_EXTENSIONS: ")+s);
	getErrors("GlErrorTool::logVersionStrings");
}

}
