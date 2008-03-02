######################################################################
# 
######################################################################

TEMPLATE = app
TARGET = StatisticsRoom
DEPENDPATH += . src src/ui src/model
INCLUDEPATH += boost/tr1/tr1 . src/ui src/model

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

