/*
 *  logwidget.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 28.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QDockWidget>

#include <memory>

class QTextEdit;
class QTextCharFormat;

namespace Examination
{
	class Program;

class LogWidget : public QDockWidget
{
	Q_OBJECT

public:
	LogWidget(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~LogWidget();

public slots:
	void writeLog(const std::string & msg);
	void writeError(const std::string & msg);

private:
	QTextEdit * logView_;
	QTextCharFormat * logFormat_;
	QTextCharFormat * errorFormat_;
};

}

#endif
