######################################################################
# 
######################################################################

TEMPLATE = app
TARGET = StatisticsRoom
DEPENDPATH += . src src/ui src/model
INCLUDEPATH += boost/tr1/tr1 . src src/ui src/model 

# Input
HEADERS += \
	src/model/logmodel.h \
	src/model/logline.h \
	src/tools/pattern.h \
	src/ui/mainwindow.h
SOURCES += \
	src/model/logmodel.cpp \
	src/model/logline.cpp \
	src/tools/pattern.cpp \
	src/ui/mainwindow.cpp \
	src/main.cpp

