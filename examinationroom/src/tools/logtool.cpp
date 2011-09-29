/*
 *  logtool.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.12.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "logtool.h"

#include <QDateTime>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "consolewidget.h"

namespace Examination
{

static const char * dateTimeFormatLogOut = "hh:mm:ss.zzz";
static const char * dateTimeFormatLogFile = "yyyy.MM.dd hh:mm:ss.zzz";
static const char * logFileFormatString = "'ExaminationRoom-'yyyy.MM.dd-hh.mm.ss.'log.txt'";

static const char * errorTypeLog = "LOG";

ConsoleWidget * LogTool::consoleWidget_;
std::ofstream LogTool::logOutStream_;

std::string LogTool::formatStringOut(const std::string & tName, const std::string & fName, const std::string & message)
{
	QDateTime t = QDateTime::currentDateTime();
	std::stringstream ss(std::ios_base::out | std::ios_base::app);
	ss << t.toString(dateTimeFormatLogOut).toStdString();
	ss << ": [" << tName << "][" << fName << "] " << message << std::endl;
	return ss.str();
}

std::string LogTool::formatStringFile(const std::string & tName, const std::string & fName, const std::string & message)
{
	QDateTime t = QDateTime::currentDateTime();
	std::stringstream ss(std::ios_base::out | std::ios_base::app);
	ss << t.toString(dateTimeFormatLogFile).toStdString();
	ss << ": [" << tName << "][" << fName << "] " << message << std::endl;
	return ss.str();
}

void LogTool::logMessage(const std::string & fName, const std::string & message)
{
	if (logOutStream_.is_open())
	{
		logOutStream_ << formatStringFile(errorTypeLog, fName, message);
	}
	if (consoleWidget_)
	{
		consoleWidget_->writeLog(formatStringOut(errorTypeLog, fName, message));
	}
	else
	{
		logStdOut(formatStringOut(errorTypeLog, fName, message));
	}
}

void LogTool::logError(const std::string & tName, const std::string & fName, const std::string & error)
{
	if (logOutStream_.is_open())
	{
		logOutStream_ << formatStringFile(tName, fName, error);
	}
	if (consoleWidget_)
	{
		consoleWidget_->writeError(formatStringOut(tName, fName, error));
	}
	else
	{
		logStdErr(formatStringOut(tName, fName, error));
	}
}

void LogTool::initLogFile()
{
	QDateTime t = QDateTime::currentDateTime();
	logOutStream_.open(t.toString(logFileFormatString).toAscii());
}

void LogTool::closeLogFile()
{
	logOutStream_.close();
}

ConsoleWidget * LogTool::consoleWidget()
{
	return consoleWidget_;
}

void LogTool::setConsoleWidget(ConsoleWidget * widget)
{
	consoleWidget_ = widget;
}

void LogTool::logStdOut(const std::string & str)
{
	std::cout << str << std::endl;
}

void LogTool::logStdErr(const std::string & str)
{
	std::cerr << str << std::endl;
}

}
