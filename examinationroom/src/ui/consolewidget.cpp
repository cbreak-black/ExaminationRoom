/*
 *  consolewidget.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 28.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "consolewidget.h"

#include "program.h"

#include <QTextEdit>
#include <QTextDocument>
#include <QVBoxLayout>
#include <QTextCharFormat>

namespace Examination
{

ConsoleWidget::ConsoleWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags)
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

ConsoleWidget::~ConsoleWidget()
{
	delete logFormat_;
	delete errorFormat_;
}

// Accessors
void ConsoleWidget::writeLog(const std::string & msg)
{
	logView_->textCursor().insertText(QString::fromStdString(msg), *logFormat_);
	logView_->ensureCursorVisible();
}

void ConsoleWidget::writeError(const std::string & msg)
{
	logView_->textCursor().insertText(QString::fromStdString(msg), *errorFormat_);
	logView_->ensureCursorVisible();
}


}
