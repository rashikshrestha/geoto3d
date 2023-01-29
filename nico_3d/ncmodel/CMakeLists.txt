PROJECT(ncmodel)

cmake_minimum_required( VERSION 2.6 )
INCLUDE(../all/CMakeLists.txt)

if(WIN32)

SET(ncmodel_HEADERS
ncmodel.h
ncassimp.h
ncmdl.h
#ncskp.h
)

SET(ncmodel_SOURCES
ncmodel.cpp
ncassimp.cpp
ncmdl.cpp
#ncskp.cpp
)

else()

SET(ncmodel_HEADERS
ncmodel.h
ncassimp.h
ncmdl.h
ncgvm.h
)

SET(ncmodel_SOURCES
ncmodel.cpp
ncassimp.cpp
ncmdl.cpp
ncgvm.cpp
)

endif()



prepend(ncmodel_HEADERS ./inc)
prepend(ncmodel_SOURCES ./src)

SET(ncmodel_RESOURCES ./ncmodel_shaders.qrc)

QT4_WRAP_CPP(ncmodel_HEADERS_MOC ${ncmodel_HEADERS})
QT4_ADD_RESOURCES(ncmodel_RESOURCES_RCC ${ncmodel_RESOURCES})

ADD_library(ncmodel STATIC ${ncmodel_SOURCES} ${ncmodel_HEADERS} ${ncmodel_RESOURCES_RCC})


