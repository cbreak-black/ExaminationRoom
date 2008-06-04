#!/bin/sh

# Changes library paths to executable relative versions
# Used in deployment with private frameworks inside
# the application bundle

echo Changing $*
echo === old ===
otool -L $*
# Change the paths to QtCore, QtGui and QtOpenGL
install_name_tool -change QtCore.framework/Versions/4/QtCore \
	@executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore \
	$*
install_name_tool -change QtGui.framework/Versions/4/QtGui \
	@executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui \
	$*
install_name_tool -change QtOpenGL.framework/Versions/4/QtOpenGL \
	@executable_path/../Frameworks/QtOpenGL.framework/Versions/4.0/QtOpenGL \
	$*
echo === new ===
otool -L $*
