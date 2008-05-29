/*
 *  codewidget.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 29.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef CODEWIDGET_H
#define CODEWIDGET_H

#include <QDockWidget>

#include <memory>

class QListWidget;
class QListWidgetItem;
class QTextEdit;

namespace Examination
{
	class Program;

class CodeWidget : public QDockWidget
{
	Q_OBJECT

public:
	CodeWidget(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);

public:
	std::tr1::shared_ptr<Program> program() const;
	void setProgram(std::tr1::shared_ptr<Program> program);

public slots:
	void fileLoaded(const std::string & path);

	void newFile();
	void addFile();
	void removeFile();

	void editFile(QListWidgetItem *);
	void previewFile(QListWidgetItem *);

private:
	std::tr1::weak_ptr<Program> program_;

	QListWidget * fileListView_;
	QTextEdit * filePreview_;
};
	
}

#endif
