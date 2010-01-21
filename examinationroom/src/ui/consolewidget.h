/*
 *  consolewidget.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 28.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QDockWidget>

#include <memory>

class QTextEdit;
class QTextCharFormat;
class QLineEdit;
class QStringListModel;
class QCompleter;

namespace Examination
{
	class Program;

class ConsoleWidget : public QDockWidget
{
	Q_OBJECT

public:
	ConsoleWidget(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~ConsoleWidget();

public:
	std::tr1::shared_ptr<Program> program() const;
	void setProgram(std::tr1::shared_ptr<Program> program);

public slots:
	void writeLog(const std::string & msg);
	void writeError(const std::string & msg);

private slots:
	void inputSubmit();

private:
	std::tr1::shared_ptr<Program> program_;

	QTextEdit * logView_;
	QTextCharFormat * logFormat_;
	QTextCharFormat * errorFormat_;
	QLineEdit * inputLine_;
	QStringListModel * inputHistory_;
	QCompleter * inputCompleter_;
};

}

#endif
