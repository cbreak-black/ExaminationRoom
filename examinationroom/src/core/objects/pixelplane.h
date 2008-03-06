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

#include <memory>

#include "object.h"

namespace Examination
{
	class AbstractTexture;
	
class Pixelplane : public Object
{
public: // Construction
	Pixelplane();
	~Pixelplane();

public:
	virtual void draw(GLWidget * dest) const;

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

	void resizeToCurrent() const;
	void resizeTo(int w, int h) const;

private:
	bool autoresize_;
	float width_;
	float height_;
};
	
}

#endif
