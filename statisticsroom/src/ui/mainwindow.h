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
#include <QTableView>

#include <memory>

namespace Statistics
{
	class LogModel;

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
	std::tr1::shared_ptr<LogModel> logModel() const;
	void setLogModel(std::tr1::shared_ptr<LogModel> lm);

private:
	std::tr1::shared_ptr<LogModel> logModel_;
	QTableView * tableView_;
};

}

#endif
