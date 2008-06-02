######################################################################
# 
######################################################################

TEMPLATE = app
TARGET = StatisticsRoom
DEPENDPATH += . src src/ui src/model src/tools
INCLUDEPATH += boost/tr1/tr1 . src src/ui src/model src/tools

# Input
HEADERS += \
	src/model/*.h \
	src/tools/pattern.h \
	src/ui/mainwindow.h
SOURCES += \
	src/model/*.cpp \
	src/tools/pattern.cpp \
	src/ui/mainwindow.cpp \
	src/main.cpp

