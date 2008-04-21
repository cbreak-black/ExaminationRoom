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
	/**
	Creates an empty text at the origin.
	The default name is "Text".
	*/
	Text();

public:
	virtual void draw(GLWidget * dest) const;

public:
	/**
	Returns the text this object displays.
	 \return the text this object displays.
	*/
	QString text() const;

	/**
	Sets the text this object displays.
	This method also changes the name to the passed text
	 \param t	A QString object
	*/
	void setText(QString t);

	/**
	Sets the text this object displays from a c string (char array).
	 \param c	A zero delimited char array pointer (standard c string)
	*/
	void setText(const char * c);

private:
	QString text_;
	QFont font_;
};
	
}

#endif
