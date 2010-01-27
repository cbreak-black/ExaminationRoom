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
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QLineEdit>
#include <QStringListModel>
#include <QCompleter>

namespace Examination
{

ConsoleWidget::ConsoleWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags)
	: QDockWidget(title, parent, flags)
{
	QWidget * mainWidget = new QWidget(this);
	setWidget(mainWidget);
	QVBoxLayout * mainLayout = new QVBoxLayout();
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainWidget->setLayout(mainLayout);

	logView_ = new QTextEdit(this);
	logView_->setReadOnly(true);
	logView_->setFontFamily("Courier");
	logView_->document()->setMaximumBlockCount(512);
	mainLayout->addWidget(logView_);

	inputLine_ = new QLineEdit(this);
	mainLayout->addWidget(inputLine_);

	connect(inputLine_, SIGNAL(returnPressed()), this, SLOT(inputSubmit()));

	inputHistory_ = new QStringListModel();
	inputCompleter_ = new QCompleter();
	inputCompleter_->setModel(inputHistory_);
	inputCompleter_->setCompletionMode(QCompleter::InlineCompletion);
	inputLine_->setCompleter(inputCompleter_);

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
	delete inputHistory_;
	delete inputCompleter_;
}

// Accessors

std::tr1::shared_ptr<Program> ConsoleWidget::program() const
{
	return program_;
}

void ConsoleWidget::setProgram(std::tr1::shared_ptr<Program> program)
{
	program_ = program;
}

// Console
void ConsoleWidget::inputSubmit()
{
	QString s = inputLine_->text();
	inputHistory_->insertRows(0, 1);
	inputHistory_->setData(inputHistory_->index(0, 0), s);
	inputHistory_->removeRows(16, 1);
	inputLine_->clear();
	if (program_)
	{
		program_->runString(s.toStdString());
	}
}

// Accessors
void ConsoleWidget::writeLog(const std::string & msg)
{
	logView_->moveCursor(QTextCursor::End);
	logView_->textCursor().insertText(QString::fromStdString(msg), *logFormat_);
	logView_->ensureCursorVisible();
}

void ConsoleWidget::writeError(const std::string & msg)
{
	logView_->moveCursor(QTextCursor::End);
	logView_->textCursor().insertText(QString::fromStdString(msg), *errorFormat_);
	logView_->ensureCursorVisible();
}


}
