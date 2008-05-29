/*
 *  logwidget.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 28.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "logwidget.h"

#include "program.h"

#include <QTextEdit>
#include <QTextDocument>
#include <QVBoxLayout>
#include <QTextCharFormat>

namespace Examination
{

LogWidget::LogWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags)
	: QDockWidget(title, parent, flags)
{
	logView_ = new QTextEdit(this);
	logView_->setReadOnly(true);
	logView_->setFontFamily("Courier");
	logView_->document()->setMaximumBlockCount(128);
	setWidget(logView_);

	logFormat_ = new QTextCharFormat();
	errorFormat_ = new QTextCharFormat();
	logFormat_->setFontFamily("Courier");
	logFormat_->setFontPointSize(11);
	errorFormat_->setFontFamily("Courier");
	errorFormat_->setFontPointSize(11);
	errorFormat_->setForeground(QBrush(QColor(255, 0, 0, 255)));
}

LogWidget::~LogWidget()
{
	delete logFormat_;
	delete errorFormat_;
}

// Accessors
std::tr1::shared_ptr<Program> LogWidget::program() const
{
	return program_.lock();
}

void LogWidget::setProgram(std::tr1::shared_ptr<Program> program)
{
	program_ = program;
	program->setCallbackLog(std::tr1::bind(&LogWidget::writeLog, this, _1));
	program->setCallbackError(std::tr1::bind(&LogWidget::writeError, this, _1));
}

void LogWidget::writeLog(const std::string & msg)
{
	logView_->textCursor().insertText(QString::fromStdString(msg), *logFormat_);
	logView_->ensureCursorVisible();
}

void LogWidget::writeError(const std::string & msg)
{
	logView_->textCursor().insertText(QString::fromStdString(msg), *errorFormat_);
	logView_->ensureCursorVisible();
}


}
