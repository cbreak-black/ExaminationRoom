/*
 *  fragshaderrenderer.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 19.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "abstractrenderer.h"

#include "helper/glshader.h"

class QGLFramebufferObject;

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

	~FragShaderRenderer();

public:
	virtual void renderScene(GLWidget * w);

private:
	void createFBO(QSize s); /**< Creates new FBOs when needed */
	void renderFBO(GLWidget * w, QGLFramebufferObject * tex); /**< Renders into the FBO */
	void drawFB(QGLFramebufferObject * tex);

private:
	GLShader shader_;
	GLint uniformSide_;

	QSize texSize_;
	QGLFramebufferObject * texL_;
	QGLFramebufferObject * texR_;

private:
	static const QString defaultShader;
};

}
