/*
 *  glwidget.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include <memory>

namespace Examination
{
	class Program;
	class AbstractRenderer;

/**
The GLWidget is a view that is drawn with OpenGL. It has a reference to a program,
who's scene is drawn into the view.

 \author Gerhard Roethlin
*/
class GLWidget : public QGLWidget
{
public: // Enums
	typedef enum {single, anaglyph, matrix, sidebyside, quad, line } DrawStyle;
	typedef enum {left = 1, right = 2} Side;

public:
	GLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
	GLWidget(const QGLFormat & format, QWidget *parent = 0, QGLWidget *shareWidget = 0);
	virtual ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public:
	std::tr1::shared_ptr<Program> program();
	void setProgram(std::tr1::shared_ptr<Program> s);

	Side side();
	void setSide(Side s);
	
	DrawStyle style();
	void setStyle(DrawStyle s);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

protected:
	virtual void keyPressEvent(QKeyEvent * event);
	virtual void keyReleaseEvent(QKeyEvent * event);

private:
	std::tr1::shared_ptr<Program> program_;
	std::tr1::shared_ptr<AbstractRenderer> renderer_;
	Side side_;
	DrawStyle style_;
};

}

#endif
