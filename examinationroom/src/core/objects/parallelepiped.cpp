/*
 *  rectangle.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 21.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#include "parallelepiped.h"

#include "surfaces/abstracttexture.h"
#include "glwidget.h"
#include "scene.h"

#include "screenproject.h"
#include "platform_math.h"

namespace Examination
{
	using namespace Tool;
	
Parallelepiped::Parallelepiped()
{
	directionC_.z = 1;
}

// Drawing
void Parallelepiped::draw(GLWidget * dest) const
{
	Point v1 = position();
	Point v2 = position() + dirB();
	Point v3 = position() + dirA();
	Point v4 = position() + dirA() + dirB();
	Point v5 = v1 + dirC();
	Point v6 = v2 + dirC();
	Point v7 = v3 + dirC();
	Point v8 = v4 + dirC();

	if (texture())
	{
		texture()->glBindTex(dest);
	}

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(texA().x, texA().y); glVertex3fv(v1.vec);
	glTexCoord2f(texB().x, texB().y); glVertex3fv(v2.vec);
	glTexCoord2f(texC().x, texC().y); glVertex3fv(v3.vec);
	glTexCoord2f(texD().x, texD().y); glVertex3fv(v4.vec);
	glTexCoord2f(texA().x, texA().y); glVertex3fv(v7.vec);
	glTexCoord2f(texB().x, texB().y); glVertex3fv(v8.vec);
	glTexCoord2f(texC().x, texC().y); glVertex3fv(v5.vec);
	glTexCoord2f(texD().x, texD().y); glVertex3fv(v6.vec);
	glTexCoord2f(texA().x, texA().y); glVertex3fv(v1.vec);
	glTexCoord2f(texB().x, texB().y); glVertex3fv(v2.vec);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(texA().x, texA().y); glVertex3fv(v1.vec);
	glTexCoord2f(texC().x, texC().y); glVertex3fv(v3.vec);
	glTexCoord2f(texB().x, texB().y); glVertex3fv(v5.vec);
	glTexCoord2f(texD().x, texD().y); glVertex3fv(v7.vec);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(texA().x, texA().y); glVertex3fv(v2.vec);
	glTexCoord2f(texC().x, texC().y); glVertex3fv(v4.vec);
	glTexCoord2f(texB().x, texB().y); glVertex3fv(v6.vec);
	glTexCoord2f(texD().x, texD().y); glVertex3fv(v8.vec);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

Tool::Vector Parallelepiped::dirC() const
{
	return directionC_;
}

void Parallelepiped::setDirC(Tool::Vector v)
{
	directionC_ = v;
}
	
}
