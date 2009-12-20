/*
 *  glerrortool.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 01.12.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GLERRORTOOL_H
#define GLERRORTOOL_H

#include <string>

namespace Examination
{

/**
Static class that handles various error related activities
*/
class GlErrorTool
{
public:
	/**
	Writes OpenGL Error Messages to stderr
	 \param fName	String containing the current function name/place
	 \param oName	String containing a current object identifier
	 \return		Number of errors reported
	*/
	static unsigned int getErrors(const std::string & fName, const std::string & oName = std::string());

	/**
	Gets the OpenGL Version data strings and writes them to the log
	*/
	static void logVersionStrings();
};

}

#endif
