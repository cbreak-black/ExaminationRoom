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
	FragShaderRenderer(std::tr1::shared_ptr<Scene> scene, QString shaderPath = defaultShader);

public:
	virtual void renderScene(GLWidget * w);

private:
	GLShader _shader;
	GLint _uniformVar;

private:
	static const QString defaultShader;
};

}
