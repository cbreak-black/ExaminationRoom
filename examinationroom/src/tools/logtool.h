/*
 *  logtool.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.12.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef LOGTOOL_H
#define LOGTOOL_H

#include <string>
#include <fstream>

namespace Examination
{

class ConsoleWidget;

/**
Static class that handles various log related activities
*/
class LogTool
{
public:
	/**
	 Writes the passed message to the log. Asumes error type "LOG"
	 \param fName	String containing the current function name/place
	 \param message	Message string
	 */
	static void logMessage(const std::string & fName, const std::string & message);

	/**
	Writes the passed error to the log as error
	 \param tName	String containing a the error type
	 \param fName	String containing the current function name/place
	 \param error	Error string
	*/
	static void logError(const std::string & tName, const std::string & fName, const std::string & error);

public: // Setup
	/**
	Returns the LogWidget instance that this logger is bound to, if available.
	 \return	shared_ptr to the currently set LogWidget if available
	*/
	static ConsoleWidget * consoleWidget();

	/**
	Sets the log passed log widget to be used for logging. Make sure to remove a log widget
	by setting the null pointer when a widget becomes invalid.
	*/
	static void setConsoleWidget(ConsoleWidget * widget);

	/**
	Initializes the log file state. Logging will be done to that file in addition to the log widget
	or the standard output.
	*/
	static void initLogFile();

	/**
	Closes the log file, flushing all state. Logging will be done only to the log widget or
	the standard output.
	*/
	static void closeLogFile();

private:
	static std::string formatStringFile(const std::string & tName, const std::string & fName, const std::string & message);
	static std::string formatStringOut(const std::string & tName, const std::string & fName, const std::string & message);
	static void logStdOut(const std::string & str);
	static void logStdErr(const std::string & str);

private:
	static ConsoleWidget * consoleWidget_;
	static std::ofstream logOutStream_;
};

}

#endif
