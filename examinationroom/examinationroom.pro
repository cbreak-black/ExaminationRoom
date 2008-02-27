######################################################################
# Manually edited by CBreak
######################################################################

TEMPLATE = app
TARGET = ExaminationRoom
DEPENDPATH += . src src/ui src/core src/lua src/tools src/proxy
INCLUDEPATH += boost/tr1/tr1 . src/ui src/core src/lua src/lua/src src/tools src/proxy

QT           += opengl

# Input
HEADERS += \
	src/ui/glwidget.h \
	src/ui/mainwindow.h \
	src/core/scene.h \
	src/core/camera.h
SOURCES += \
	src/main.cpp \
	src/ui/glwidget.cpp \
	src/ui/mainwindow.cpp \
	src/core/scene.cpp \
	src/core/camera.cpp

# Surfaces
HEADERS += src/core/surfaces/*.h
SOURCES += src/core/surfaces/*.cpp

# Objects
HEADERS += src/core/objects/*.h
SOURCES += src/core/objects/*.cpp

# Proxies
HEADERS += src/proxy/*.h
SOURCES += src/proxy/*.cpp

# LUA
HEADERS += src/lua/src/*.h src/lua/lua.hpp
SOURCES += src/lua/src/*.c

# LUNA
HEADERS += src/lua/luna.h

# TOOLS
HEADERS += \
	src/tools/luahelper.h \
	src/tools/platform.h \
	src/tools/platform_math.h \
	src/tools/screenproject.h \
	src/tools/vec.h
SOURCES += \
	src/tools/screenproject.cpp \
	src/tools/luahelper.cpp

