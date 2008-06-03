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
#include "logtransformermodel.h"
#include "pattern.h"

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

	QSplitter * split = new QSplitter(Qt::Vertical);
	mainLayout->addWidget(split, 1, 0, 1, 6);

	patternView_ = new QTreeView();
	patternView_->setIndentation(0);
	patternView_->setUniformRowHeights(true);
	split->addWidget(patternView_);

	tableView_ = new QTreeView();
	tableView_->setIndentation(0);
	tableView_->setUniformRowHeights(true);
	split->addWidget(tableView_);

	QPushButton * bLoad = new QPushButton(tr("Load..."));
	QPushButton * bStore = new QPushButton(tr("Store..."));
	QPushButton * bTransform = new QPushButton(tr("Transform..."));
	QPushButton * bPlus = new QPushButton("+");
	QPushButton * bMinus = new QPushButton("-");
	mainLayout->addWidget(bLoad, 0, 0);
	mainLayout->addWidget(bStore, 0, 1);
	mainLayout->addWidget(bPlus, 0, 2);
	mainLayout->addWidget(bMinus, 0, 3);
	mainLayout->addWidget(bTransform, 0, 5);

	connect(bLoad, SIGNAL(clicked()), this, SLOT(loadClicked()));
	connect(bStore, SIGNAL(clicked()), this, SLOT(storeClicked()));
	connect(bTransform, SIGNAL(clicked()), this, SLOT(transformClicked()));
	connect(bPlus, SIGNAL(clicked()), this, SLOT(plusClicked()));
	connect(bMinus, SIGNAL(clicked()), this, SLOT(minusClicked()));

	logTransformer_ = std::tr1::shared_ptr<LogTransformer>(new LogTransformer());

	// Model
	setLogModel(shared_ptr<LogModel>(new LogModel()));
	patternView_->setModel(new LogTransformerModel(logTransformer_));

	// Sizing
	patternView_->setColumnWidth(0, 400);
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
void MainWindow::loadClicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Pattern File"), QString(), tr("Pattern Files (*.pattern)"));
	if (!fileName.isNull())
	{
		QFile f(fileName);
		f.open(QIODevice::ReadOnly | QIODevice::Text);
		QTextStream s(&f);
		logTransformer_->loadFromStream(s);
		patternView_->reset();
	}
}

void MainWindow::storeClicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Load Pattern File"), QString(), tr("Pattern Files (*.pattern)"));
	if (!fileName.isNull())
	{
		QFile f(fileName);
		f.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream s(&f);
		logTransformer_->writeToStream(s);
	}
}

void MainWindow::transformClicked()
{
	QString fileNameLoad = QFileDialog::getOpenFileName(this, tr("Load Log File"), QString(), tr("Log File ( *.log *.txt)"));
	QString fileNameStore = QFileDialog::getSaveFileName(this, tr("Save Statistics File"), QString(), tr("Tab separated list (*.csv)"));
	if (!fileNameLoad.isNull() && !fileNameStore.isNull())
	{
		QFile iFile(fileNameLoad);
		QFile oFile(fileNameStore);
		iFile.open(QIODevice::ReadOnly | QIODevice::Text);
		oFile.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream in(&iFile);
		QTextStream out(&oFile);
		setLog(Log::logFromStream(in));
		logTransformer_->transformLog(log(), out);
	}
}

void MainWindow::plusClicked()
{
	int idx = patternView_->currentIndex().row();
	logTransformer_->insertStimulusData(idx-1, std::tr1::shared_ptr<Pattern>(new Pattern()));
	patternView_->reset();
}

void MainWindow::minusClicked()
{
	int idx = patternView_->currentIndex().row();
	logTransformer_->removeStimulusData(idx-1);
	patternView_->reset();
}

}
