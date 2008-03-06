/*
 *  text.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 25.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef TEXT_H
#define TEXT_H

#include "object.h"

#include <QString>
#include <QFont>

namespace Examination
{

/**
This class represents a renderable piece of text.
*/
class Text : public Object
{
public: // Construction
	Text();

public:
	virtual void draw(GLWidget * dest) const;

public:
	QString text() const;
	void setText(QString t);
	void setText(const char * c);

private:
	QString text_;
	QFont font_;
};
	
}

#endif
