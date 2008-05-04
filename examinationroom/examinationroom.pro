######################################################################
# Manually edited by CBreak
######################################################################

TEMPLATE = app
TARGET = ExaminationRoom
DEPENDPATH += . src/ui src/core src/lua src/tools src/proxy
INCLUDEPATH += \
	boost boost/tr1/tr1 \
	. src \
	src/ui src/core src/tools src/proxy \
	src/lua src/lua/src src/parser

QT           += opengl

# Input
HEADERS += \
	src/core/scene.h \
	src/core/camera.h
SOURCES += \
	src/main.cpp \
	src/core/scene.cpp \
	src/core/camera.cpp

# UI
HEADERS += \
	src/ui/*.h \
	src/ui/helper/*.h \
	src/ui/parameter/*.h
SOURCES += \
	src/ui/*.cpp \
	src/ui/helper/*.cpp \
	src/ui/parameter/*.cpp

# Surfaces
HEADERS += src/core/surfaces/*.h
SOURCES += src/core/surfaces/*.cpp

# Objects
HEADERS += src/core/objects/*.h
SOURCES += src/core/objects/*.cpp

# Proxies
HEADERS += src/proxy/*.h
SOURCES += src/proxy/*.cpp

# Parsers
HEADERS += src/parser/obj/*.hpp src/parser/*.hpp
SOURCES += src/parser/obj/*.cpp

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
	src/tools/platform_string.h \
	src/tools/screenproject.h \
	src/tools/vec.h
SOURCES += \
	src/tools/screenproject.cpp \
	src/tools/luahelper.cpp

