######################################################################
# 
######################################################################

TEMPLATE = app
TARGET = StatisticsRoom
DEPENDPATH += . src src/ui
INCLUDEPATH += . src/ui

# Input
HEADERS += \
	src/ui/mainwindow.h
SOURCES += \
	src/ui/mainwindow.cpp \
	src/main.cpp

