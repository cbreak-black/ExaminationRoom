######################################################################
# 
######################################################################

TEMPLATE = app
TARGET = StatisticsRoom
DEPENDPATH += . src src/ui
INCLUDEPATH += . src/ui

# Input
HEADERS += \
	src/model/logmodel.h \
	src/model/logline.h \
	src/ui/mainwindow.h
SOURCES += \
	src/model/logmodel.cpp \
	src/model/logline.cpp \
	src/ui/mainwindow.cpp \
	src/main.cpp

