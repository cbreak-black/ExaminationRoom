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
	src/lua src/lua/src src/lua/luabridge/include src/parser

QT           += opengl

QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
CONFIG+=x86 ppc

# Core
SOURCES += src/main.cpp
HEADERS += src/core/*.h
SOURCES += src/core/*.cpp

# UI
HEADERS += \
	src/ui/*.h \
	src/ui/helper/*.h \
	src/ui/parameter/*.h \
	src/ui/renderer/*.h
SOURCES += \
	src/ui/*.cpp \
	src/ui/helper/*.cpp \
	src/ui/parameter/*.cpp \
	src/ui/renderer/*.cpp

# Surfaces
HEADERS += src/core/surfaces/*.h
SOURCES += src/core/surfaces/*.cpp

# Objects
HEADERS += src/core/objects/*.h
SOURCES += src/core/objects/*.cpp

# Proxies
HEADERS += src/proxy/luaproxy.h
SOURCES += src/proxy/luaproxy.cpp

# Parsers
HEADERS += src/parser/obj/*.hpp src/parser/*.hpp
SOURCES += src/parser/obj/*.cpp

# LUA
HEADERS += src/lua/src/*.h src/lua/lua.hpp
SOURCES += src/lua/src/*.c

# LUABIND
HEADERS += \
	src/lua/luabridge/include/luabridge.hpp \
	src/lua/luabridge/include/impl/*.hpp
SOURCES += src/lua/luabridge/src/luabridge.cpp

# TOOLS
HEADERS += \
	src/tools/errortool.h \
	src/tools/luahelper.h \
	src/tools/platform.h \
	src/tools/platform_math.h \
	src/tools/platform_string.h \
	src/tools/screenproject.h \
	src/tools/vec.h
SOURCES += \
	src/tools/errortool.cpp \
	src/tools/screenproject.cpp \
	src/tools/luahelper.cpp

