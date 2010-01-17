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

#include <QFont>

class QGLFramebufferObject;

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

	/**
	Creates a clone of a text object.
	 \param t	Text object to clone
	 \warning Don't create instances of this class directly, use Object::Create()
	*/
	Text(const Text & t);

	/**
	Destructor.
	*/
	~Text();

public: // Cloning
	/**
	Returns a newly allocated clone of this object.
	 \return a newly created copy of this object
	*/
	virtual ObjectPtr clone() const;

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
	std::string text() const;

	/**
	Sets the text this object displays.
	This method also changes the name to the passed text
	 \param t	A std::string object
	*/
	void setText(std::string t);

	/**
	Sets the text this object displays from a c string (char array).
	 \param c	A zero delimited char array pointer (standard c string)
	*/
	void setText(const char * c);

	/**
	Returns the maximal dimension of this string on screen in pixel.
	 \return the maximal size of this text in pixel
	*/
	Tool::Vec2f dimensions() const;

	/**
	Sets the maximal dimension of the string on screen in pixel.
	 \param b	The Bounding box
	*/
	void setDimensions(const Tool::Vec2f & d);

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

public: // LUA API
	static void registerLuaApi(luabridge::module * m);

protected: // Parameter Dialog
	virtual std::tr1::shared_ptr<ParameterObject> createDialog();

private: // Caching
	void updateRenderedString() const;
	void drawRenderedString() const;

private:
	std::string text_;
	QFont font_;
	Tool::Vec2f dimensions_;

	// Caching/Rendering
	mutable Tool::Vec2f renderedDimensions_;
	mutable bool renderedStringValid_;
	mutable QGLFramebufferObject * renderedString_;

public: // Meta
	static const char * className_;
};
	
}

#endif
