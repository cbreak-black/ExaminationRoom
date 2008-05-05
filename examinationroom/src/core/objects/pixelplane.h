/*
 *  pixelplane.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 12.02.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef PIXELPLANE_H
#define PIXELPLANE_H

#include "object.h"

namespace Examination
{
	class AbstractTexture;

/**
A Pixelplane is an object that only consists of a texture. The texture is drawn
at a raster position in space. It obeys depth testing but not any kind of transformation.
Only the raster position itself is transformed.
 \author Gerhard Roethlin
 \ingroup Objects
*/
class Pixelplane : public Object
{
public: // Construction
	/**
	Creates a Pixelplane.
	Default size is 1 by 1 unit, default zoom factors are 1 by 1, default name
	is "Pixelplane". autoResize() is disabled.
	 \warning Don't create instances of this class directly, use Object::Create()
	 \see Object::Create()
	*/
	Pixelplane();

public:
	virtual void draw(GLWidget * dest) const;

public:
	/**
	Returns the size of the pixelplane.
	 \see setSize()
	 \return	A Vec2f containing the size of the Pixelplane
	 */
	Tool::Vec2f size() const;

	/**
	Sets the intended size of the pixelplane. This is used to rescales the associated
	Texture object according to perspective projection, if autoResize() is enabled
	or resizeToCurrent() is called.
	 \param w	Width of the pixel plane in world space units
	 \param h	Height of the pixel plane in world space units
	*/
	void setSize(float w, float h);

	/**
	Sets the auto resize status. Auto resizing automatically scales
	textures to correspond pixel-by-pixel to the rendered size.
	 \param b	Boolean indicating the desired auto resize state
	 */
	virtual void setAutoResize(bool b);

	/**
	Returns the auto resize status.
	 \return the auto resize status
	 */
	virtual bool autoResize() const;

	/**
	Resizes the pixel plane to have the correct size according to the current position
	and projection matrix. This might not work well when called on an object in a
	CameraNode, since the it uses the projection and modelview matrix of the scene.
	 \todo Use camera of parent if needed
	*/
	void resizeToCurrent() const;

	/**
	Resizes the pixel plane to the given size. This is equivalent to calling
	the same function on the contained texture.
	 \see AbstractTexture::resizeTo(int,int)
	 \param w	Width in pixel
	 \param h	Height in pixel
	*/
	void resizeTo(int w, int h) const;

	/**
	Returns the zoom factors of the Pixelplane.
	 \return the zoom factors of the Pixelplane.
	*/
	Tool::Vec2f zoom() const;

	/**
	Changes the zoom factor of the Pixelplane. This also changes the zoom factor
	of associated textures. When new textures are set, they are zoomed as well.
	 \see	AbstractTexture::setZoom(float,float)
	 \param zx	Zoom factor in x direction
	 \param zy	Zoom factor in y direction
	*/
	void setZoom(float zx, float zy);

	virtual void setTexture(std::tr1::shared_ptr<AbstractTexture> t);

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
	bool autoresize_;
	float width_;
	float height_;
	float zoomFactorX_;
	float zoomFactorY_;
};
	
}

#endif
