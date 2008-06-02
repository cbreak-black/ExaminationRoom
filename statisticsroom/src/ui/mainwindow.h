/*
 *  mainwindow.h
 *  StatisticsRoom
 *
 *  Created by CBreak on 29.02.08
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <memory>

class QTreeView;

namespace Statistics
{
	class Log;
	class LogModel;
	class LogTransformer;

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	/**
	Creates the MainWindow and the environment.
	*/
    MainWindow();
	
	/**
	Destroys the the MainWindow and the environment.
	*/
	~MainWindow();

public:
	QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
	void loadClicked(bool checked);
	void exportClicked(bool checked);

private:
	std::tr1::shared_ptr<Log> log() const;
	std::tr1::shared_ptr<LogModel> logModel() const;

private:
	void setLog(std::tr1::shared_ptr<Log> log);
	void setLogModel(std::tr1::shared_ptr<LogModel> lm);

private:
	std::tr1::shared_ptr<Log> log_;
	std::tr1::shared_ptr<LogModel> logModel_;
	std::tr1::shared_ptr<LogTransformer> logTransformer_;
	QTreeView * patternView_;
	QTreeView * tableView_;
};

}

#endif
