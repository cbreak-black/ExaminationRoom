######################################################################
# Manually edited by CBreak
######################################################################

TEMPLATE = app
TARGET = ExaminationRoom
DEPENDPATH += . src src/ui src/core src/lua src/tools
INCLUDEPATH += . src/ui src/core src/lua src/tools

QT           += opengl

# Input
HEADERS += \
	src/ui/glwidget.h \
	src/ui/mainwindow.h \
	src/core/scene.h \
	src/core/object.h \
	src/core/rectangle.h \
	src/core/drawing.h \
	src/core/abstracttexture.h \
	src/core/texture.h \
	src/core/stereogram.h \
	src/proxy/luaproxy.h \
	src/tools/vec.h
SOURCES += \
	src/main.cpp \
	src/ui/glwidget.cpp \
	src/ui/mainwindow.cpp \
	src/core/scene.cpp \
	src/core/object.cpp \
	src/core/rectangle.cpp \
	src/core/abstracttexture.cpp \
	src/core/texture.cpp \
	src/core/stereogram.cpp \
	src/proxy/luaproxy.cpp


# LUA
HEADERS += src/lua/src/*.h src/lua/lua.hpp
SOURCES += src/lua/src/*.c

# LUNA
HEADERS += src/lua/luna.h
