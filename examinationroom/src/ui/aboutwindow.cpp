/*
 *  aboutwindow.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 30.05.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "aboutwindow.h"

#include <QGridLayout>
#include <QTextEdit>
#include <QPushButton>

namespace Examination
{

AboutWindow::AboutWindow(QWidget * parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	QGridLayout * layout = new QGridLayout();
	setLayout(layout);

	QTextEdit * te = new QTextEdit();
	te->setReadOnly(true);
	te->setAlignment(Qt::AlignCenter);
	layout->addWidget(te, 0, 0, 3, 3);

	te->setPlainText("Examinationroom v0.0\n");
	te->append("Examinationroom is a tool for user testing with complex and highly dynamic stimuli in a stereoscopic environment.");
	te->append("");
	te->append("(C) 2008 by Gerhard Roethlin");
	te->append("licensed under the GPL v2");
	te->append("");
	te->append("this program uses code from or links to the following projects:");
	te->append("- LUA (MIT License)");
	te->append("- LuaBridge (BSD License)");
	te->append("- libObj (GPL v2)");
	te->append("- QT (GPL v2)");
	te->append("- Boost (Boost License)");
	te->append("Thanks!");

	QPushButton * pushClose = new QPushButton("Close");
	layout->addWidget(pushClose, 3, 1);
	connect(pushClose, SIGNAL(clicked()), this, SLOT(close()));
}

}
