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

namespace Examination
{
	class Scene;

/**
The GLWidget is a view that is drawn with OpenGL. It has a reference to a scene,
which is drawn into the view.
 
 \author Gerhard Roethlin
*/
class GLWidget : public QGLWidget
{
public: // Enums
	typedef enum {single, anaglyph, sidebyside} DrawStyle;
	typedef enum {left = 1, right = 2} Side;

public:
    GLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
    virtual ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public:
	Scene * scene();
	void setScene(Scene * s);
	
	Side side();
	void setSide(Side s);
	
	DrawStyle style();
	void setStyle(DrawStyle s);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
	Scene * scene_;
	Side side_;
	DrawStyle style_;
};

}

#endif
