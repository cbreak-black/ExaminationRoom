/*
 *  glshader.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 14.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GLSHADER_H
#define GLSHADER_H

#include <qgl.h>
#include <QString>

namespace Examination
{

/**
A GLShader is a wrapper for a GL Shader Program with up to one single vertex and
up to one single fragment shader.
 */
class GLShader
{
public:
	/**
	 Construct a GLShader.
	 When constructed, shaders are all empty.
	 */
	GLShader();

	/**
	Destroys a GLShader.
	 */
	~GLShader();

public: // Using
	/**
	Enables and uses the current GLSL Program.
	*/
	void use();

	/**
	Disables any loaded GLSL Program and switches back to fixed functionality.
	*/
	static void disable();

public: // Loading Programs
	/**
	Loads and links a GLSL fragment shader stored in the file at path path.
	 /param path	A QString to the file containing the shader
	 /return		True when the reading succeeded, false otherwise
	 */
	bool loadFragmentShader(const QString & path);

	/**
	Loads and links a GLSL vertex shader stored in the file at path path.
	 /param path	A QString to the file containing the shader
	 /return		True when the reading succeeded, false otherwise
	 */
	bool loadVertexShader(const QString & path);

	/**
	Unloads any given fragment shaders in this program
	 */
	void unloadFragmentShader();

	/**
	 Unloads any given fragment shaders in this program
	 */
	void unloadVertexShader();

public: // Communicating
	GLint getUniformLocation(const char * name);
	void setUniform1f(GLint location, GLfloat value);
	void setUniform2f(GLint location, GLfloat v1, GLfloat v2);
	void setUniform3f(GLint location, GLfloat v1, GLfloat v2, GLfloat v3);
	void setUniform4f(GLint location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4);

private:
	/**
	Read the text file at path "path" and uses it as source for
	the shader shader.
	 \param shader	A shader created with glCreateShader
	 \param path	A QString pointer pointing to the path of the file containing the source
	 \return		True if the read succeeded, false otherwise
	 */
	static bool loadShaderFromFile(GLuint shader, const QString & path);

private:
	GLuint _program; /**< Handle of the GLSL Program */
	GLuint _frag; /**< Handle of the GLSL Fragment shader */
	GLuint _vert; /**< Handle of the GLSL Vertex shader */
};

}

#endif
