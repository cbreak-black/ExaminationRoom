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
 \ingroup Objects
*/
class Text : public Object
{
public: // Construction
	/**
	Creates an empty text at the origin.
	The default name is "Text".
	 \warning Don't create instances of this class directly, use Object::Create()
	 \see Object::Create()
	*/
	Text();

public:
	/**
	Draws the text to the passed widget.
	 \warning	This method uses Qt's draw method, which in turn uses the Stencil Buffer.
				This is incompatible with some rendering methods.
	 \param dest	a GLWidget to draw into
	*/
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

public: // Serialisation
	/**
	Returns the name of the class of this object. This can be used in LUA
	object creation.
	 \return The name of this object's class as c++ string
	*/
	virtual std::string className() const;

	/**
	Writes the LUA commands to set parameters of this object to the output stream.
	 \param outStream	A stream that accepts writing
	*/
	virtual std::string toLua(std::ostream & outStream) const;

private:
	QString text_;
	QFont font_;
};
	
}

#endif
