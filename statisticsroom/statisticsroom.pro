######################################################################
# 
######################################################################

TEMPLATE = app
TARGET = StatisticsRoom
DEPENDPATH += . src src/ui src/core src/model
INCLUDEPATH += boost/tr1/tr1 . src src/ui src/core src/model

# Input
HEADERS += \
	src/core/*.h \
	src/model/*.h \
	src/ui/mainwindow.h

SOURCES += \
	src/core/*.cpp \
	src/model/*.cpp \
	src/ui/mainwindow.cpp \
	src/main.cpp

