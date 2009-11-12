/*
 *  glshader.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 14.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "glshader.h"

//#include <memory.h>
#include <QFile>
#include <QString>

namespace Examination
{

GLShader::GLShader()
{
	// Create the Program
	program_ = glCreateProgram();

	frag_ = 0;
	vert_ = 0;
}

GLShader::~GLShader()
{
	glDeleteProgram(program_);
	glDeleteShader(frag_);
	glDeleteShader(vert_);
}

// Using
void GLShader::use()
{
	glUseProgram(program_);
}

void GLShader::disable()
{
	glUseProgram(0);
}

bool GLShader::loadFragmentShader(const QString & path)
{
	// There was already an old shader
	unloadFragmentShader();

	// Create the shader
	frag_ = glCreateShader(GL_FRAGMENT_SHADER);

	// Load the code
	if (loadShaderFromFile(frag_, path))
	{
		// Link the program
		glAttachShader(program_, frag_);
		glLinkProgram(program_);

		GLint state;
		glGetProgramiv(program_, GL_LINK_STATUS, &state);
		return state == GL_TRUE;
	}
	else
	{
		return false;
	}
}

bool GLShader::loadVertexShader(const QString & path)
{
	// There was already an old shader
	unloadVertexShader();

	// Load the code
	loadShaderFromFile(vert_, path);

	// Link the program
	if (loadShaderFromFile(vert_, path))
	{
		// Link the program
		glAttachShader(program_, vert_);
		glLinkProgram(program_);

		GLint state;
		glGetProgramiv(program_, GL_LINK_STATUS, &state);
		return state == GL_TRUE;
	}
	else
	{
		return false;
	}
}

void GLShader::unloadFragmentShader()
{
	if (frag_ != 0)
	{
		glDetachShader(program_, frag_);
		glDeleteShader(vert_);
		glLinkProgram(program_);
		frag_ = 0;
	}
}

void GLShader::unloadVertexShader()
{
	if (vert_ != 0)
	{
		glDetachShader(program_, vert_);
		glDeleteShader(vert_);
		glLinkProgram(program_);
		vert_ = 0;
	}
}

bool GLShader::loadShaderFromFile(GLuint shader, const QString & path)
{
	QFile qfile(path);
	if (qfile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString qstr = qfile.readAll();
		if (!qstr.isNull())
		{
			QByteArray qba = qstr.toAscii();
			const GLchar * src = qba.data();
			glShaderSource(shader, 1, &src, 0);
			glCompileShader(shader);
			GLint state;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
			return state == GL_TRUE;
		}
		else
		{
			// Read error
			return false;
		}
	}
	else
	{
		// Open error
		return false;
	}
}

GLint GLShader::getUniformLocation(const char * name)
{
	return glGetUniformLocation(program_, name);
}

void GLShader::setUniform1f(GLint location, GLfloat v1)
{
	glUniform1f(location, v1);
}
void GLShader::setUniform2f(GLint location, GLfloat v1, GLfloat v2)
{
	glUniform2f(location, v1, v2);
}

void GLShader::setUniform3f(GLint location, GLfloat v1, GLfloat v2, GLfloat v3)
{
	glUniform3f(location, v1, v2, v3);
}

void GLShader::setUniform4f(GLint location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4)
{
	glUniform4f(location, v1, v2, v3, v4);
}


}
