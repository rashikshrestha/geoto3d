cmake_minimum_required( VERSION 2.6 )
PROJECT(ncqt)

INCLUDE(../all/CMakeLists.txt)

INCLUDE_DIRECTORIES( ../ncmodel/inc )

SET(ncqt_HEADERS
ncqtabout.h
ncqtconfig.h
ncqtconfigmanager.h
ncqtglwidget.h
ncqt.h
ncqtprogram.h
ncqtcamera.h
#ncqtintslidercombo.h
ncqtprogressbar.h
ncqtdock.h
ncqtintspinbox.h
ncqtsignalhandler.h
ncqtdoubleslidercombo.h
ncqtlogger.h
ncqtvideo.h
ncqtdoublespinbox.h
ncqtmainwindow.h
ncqtxymessenger.h
ncqtscenix.h
ncqtmultibar.h
ply.h
)
prepend(ncqt_HEADERS ./include)

SET(all_HEADERS ${ncqt_HEADERS} ${style_HEADERS})

SET(ncqt_SOURCES
ncqtabout.cpp
ncqtconfig.cpp
ncqtconfigmanager.cpp
ncqtdoublespinbox.cpp
ncqtglwidget.cpp
ncqtprogram.cpp
ncqtcamera.cpp
#ncqtintslidercombo.cpp
ncqtprogressbar.cpp
ncqt.cpp
ncqtintspinbox.cpp
ncqtsignalhandler.cpp
ncqtdock.cpp
ncqtlogger.cpp
ncqtvideo.cpp
ncqtdoubleslidercombo.cpp
ncqtmainwindow.cpp
ncqtxymessenger.cpp
ncqtscenix.cpp
ncqtmultibar.cpp
ply.c
)
prepend(ncqt_SOURCES ./src)

SET(all_SOURCES ${ncqt_SOURCES} ${style_SOURCES} )

SET(ncqt_RESOURCES icons/icons.qrc ./ncqt_shaders.qrc)

QT4_WRAP_CPP(ncqt_HEADERS_MOC ${all_HEADERS})
QT4_ADD_RESOURCES(ncqt_RESOURCES_RCC ${ncqt_RESOURCES})
add_library(ncqt ${all_SOURCES} ${all_HEADERS} ${ncqt_HEADERS_MOC} ${ncqt_RESOURCES_RCC} )
