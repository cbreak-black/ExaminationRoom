/*
 *  fragshaderrenderer.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "abstractrenderer.h"

#include "glshader.h"

namespace Examination
{

/**
A renderer that renders with a fragment shader. Currently not very customizable.
*/
class FragShaderRenderer : public AbstractRenderer
{
public:
	/**
	Creates a FragShaderRenderer.
	Such a renderer uses a GLShader filled with the passed fragment shader or the default shader.
	The renderer sets the uniform float variable "side" to 0 or 1 depending on the camera.
	 \param scene	Scene ptr
	 \param shaderPath	Path to the fragment shader, defafults to line interlaced
	 */
	FragShaderRenderer(std::tr1::shared_ptr<Scene> scene, QString shaderPath = defaultShader);

public:
	virtual void renderScene(GLWidget * w);

private:
	GLShader shader_;
	GLint uniform_;

private:
	static const QString defaultShader;
};

}
