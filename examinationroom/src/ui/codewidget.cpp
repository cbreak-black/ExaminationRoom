/*
 *  codewidget.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 29.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "codewidget.h"

#include "program.h"

#include <QAction>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QUrl>
#include <QFileDialog>
#include <QFileInfo>
#include <QDesktopServices>
#include <QTextStream>

namespace Examination
{

CodeWidget::CodeWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags)
	: QDockWidget(title, parent, flags)
{
	QSplitter * splitter = new QSplitter(Qt::Vertical, this);
	setWidget(splitter);

//	QAction * newAction = new QAction(tr("New"), this);
//	QAction * addAction = new QAction(tr("Add..."), this);
//	QAction * remAction = new QAction(tr("Remove"), this);
//	remAction->setShortcut(QKeySequence::Delete);
//	connect(newAction, SIGNAL(triggered()),
//			this, SLOT(newFile()));
//	connect(addAction, SIGNAL(triggered()),
//			this, SLOT(addFile()));
//	addAction(remAction);
//	connect(remAction, SIGNAL(triggered()),
//			this, SLOT(removeFile()));

	QWidget * buttonWidget = new QWidget();
	QHBoxLayout * buttonLayout = new QHBoxLayout();
	buttonWidget->setLayout(buttonLayout);
	QPushButton * pbNew = new QPushButton(tr("New"));
	QPushButton * pbAdd = new QPushButton(tr("Add..."));
	QPushButton * pbRem = new QPushButton(tr("Remove"));
	buttonLayout->addWidget(pbNew);
	buttonLayout->addWidget(pbAdd);
	buttonLayout->addWidget(pbRem);
	connect(pbNew, SIGNAL(clicked()),
			this, SLOT(newFile()));
	connect(pbAdd, SIGNAL(clicked()),
			this, SLOT(addFile()));
	connect(pbRem, SIGNAL(clicked()),
			this, SLOT(removeFile()));

	fileListView_ = new QListWidget();
	connect(fileListView_, SIGNAL(itemActivated(QListWidgetItem *)),
			this, SLOT(editFile(QListWidgetItem *)));
	connect(fileListView_, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
			this, SLOT(previewFile(QListWidgetItem *)));

	QWidget * topWidget = new QWidget();
	QVBoxLayout * topLayout = new QVBoxLayout();
	topWidget->setLayout(topLayout);
	topLayout->addWidget(buttonWidget);
	topLayout->addWidget(fileListView_);

	filePreview_ = new QTextEdit();
	filePreview_->setReadOnly(true);
	filePreview_->setFontFamily("Courier");
	filePreview_->setFontPointSize(11);

	splitter->addWidget(topWidget);
	splitter->addWidget(filePreview_);
}

// Accessors
std::tr1::shared_ptr<Program> CodeWidget::program() const
{
	return program_.lock();
}

void CodeWidget::setProgram(std::tr1::shared_ptr<Program> program)
{
	using namespace std::tr1::placeholders;
	program_ = program;
	program->setCallbackFileLoad(std::tr1::bind(&CodeWidget::fileLoaded, this, _1));
	fileListView_->clear();
}

void CodeWidget::fileLoaded(const std::string & path)
{
	new QListWidgetItem(QString::fromStdString(path), fileListView_);
}

void CodeWidget::newFile()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save LUA File",
													"res/",
													"Lua Scene File (*.lua)");
	if (!fileName.isNull())
	{
		QFile outFile(fileName);
		outFile.open(QFile::WriteOnly | QFile::Text);
		outFile.write(QByteArray("-- Insert LUA code here\n"));
		QDir dir(QDir::currentPath());
		program()->loadLua(dir.relativeFilePath(fileName).toStdString());
	}
}

void CodeWidget::addFile()
{
	// Set up lua engine and bindings
	QString fileName = QFileDialog::getOpenFileName(this, "Open LUA File",
													"res/scene.lua",
													"Lua Scene File (*.lua)");
	if (!fileName.isNull())
	{
		QDir dir(QDir::currentPath());
		program()->loadLua(dir.relativeFilePath(fileName).toStdString());
	}
}

void CodeWidget::removeFile()
{
	QListWidgetItem * lwi = fileListView_->currentItem();
	if (program()->removeLua(lwi->text().toStdString()))
	{
		fileListView_->takeItem(fileListView_->currentRow());
		fileListView_->setCurrentRow(-1);
		delete lwi;
	}
}

void CodeWidget::editFile(QListWidgetItem * file)
{
	QFileInfo inputFileInfo(file->text());
	if (inputFileInfo.exists())
	{
		QUrl fileUrl(inputFileInfo.absoluteFilePath());
		fileUrl.setScheme("file");
		QDesktopServices::openUrl(fileUrl);
	}
}

void CodeWidget::previewFile(QListWidgetItem * file)
{
	if (file)
	{
		QFile inputFile(file->text());
		if (inputFile.open(QFile::ReadOnly | QFile::Text))
		{
			QTextStream stream(&inputFile);
			filePreview_->setPlainText(stream.readAll());
		}
	}
	else
	{
		filePreview_->clear();
	}
}


}
