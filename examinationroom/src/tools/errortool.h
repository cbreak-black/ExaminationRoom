/*
 *  errortool.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 01.12.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ERRORTOOL_H
#define ERRORTOOL_H

#include <string>

namespace Examination
{

/**
Static class that handles various error related activities
*/
class ErrorTool
{
public:
	/**
	Writes OpenGL Error Messages to stderr
	 \param fName	String containing the current function name/place
	 \param oName	String containing a current object identifier
	 \return		Number of errors reported
	*/
	static unsigned int getErrors(std::string fName, std::string oName = std::string());

	/**
	Writes the passed error to stderr
	 \param fName	String containing the current function name/place
	 \param error	Error string
	*/
	static void logError(std::string fName, std::string error);
};

}

#endif
