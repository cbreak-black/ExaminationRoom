/*
 *  mainwindow.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 29.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "mainwindow.h"

#include <QtGui>

#include "log.h"
#include "logmodel.h"
#include "logtransformer.h"

using namespace std::tr1;

namespace Statistics
{

MainWindow::MainWindow()
{
	// Visual
    QGridLayout *mainLayout = new QGridLayout;
	//mainLayout->setContentsMargins(0,0,0,0);
	//mainLayout->setSpacing(0);

	setLayout(mainLayout);

	tableView_ = new QTableView();
	tableView_->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	tableView_->horizontalHeader()->setStretchLastSection(true);
	tableView_->verticalHeader()->setVisible(false);
	mainLayout->addWidget(tableView_, 0, 0, 3, 3);
	
	QPushButton * bLoad = new QPushButton(tr("Load..."));
	QPushButton * bExport = new QPushButton(tr("Export..."));
	mainLayout->addWidget(bLoad, 3, 1);
	mainLayout->addWidget(bExport, 3, 2);
	QObject::connect(bLoad, SIGNAL(clicked(bool)), this, SLOT(loadClicked(bool)));
	QObject::connect(bExport, SIGNAL(clicked(bool)), this, SLOT(exportClicked(bool)));
	
	// Model
	setLogModel(shared_ptr<LogModel>(new LogModel()));
	logTransformer_ = std::tr1::shared_ptr<LogTransformer>(new LogTransformer());
}


MainWindow::~MainWindow()
{
}

QSize MainWindow::minimumSizeHint() const
{
	return QSize(800, 600);
}

QSize MainWindow::sizeHint() const
{
	return QSize(1024, 768);
}

shared_ptr<Log> MainWindow::log() const
{
	return log_;
}

void MainWindow::setLog(shared_ptr<Log> log)
{
	log_ = log;
	logModel_->setLog(log);
}

shared_ptr<LogModel> MainWindow::logModel() const
{
	return logModel_;
}

void MainWindow::setLogModel(shared_ptr<LogModel> lm)
{
	tableView_->setModel(lm.get());
	logModel_ = lm;
}

// Slots
void MainWindow::loadClicked(bool /* checked */)
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Log File"), QString(), tr("Log Files (*.log *.txt)"));
	if (!fileName.isNull())
	{
		QFile f(fileName);
		f.open(QIODevice::ReadOnly | QIODevice::Text);
		QTextStream s(&f);
		setLog(Log::logFromStream(s));
	}
}

void MainWindow::exportClicked(bool /* checked */)
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Statistics File"), QString(), tr("Tab seperated list (*.csv)"));
	if (!fileName.isNull())
	{
		QFile f(fileName);
		f.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream s(&f);
		logTransformer_->transformLog(log(), s);
	}
}


}
