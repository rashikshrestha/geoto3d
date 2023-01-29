PROJECT(ncvoxel)

cmake_minimum_required( VERSION 2.6 )
INCLUDE(../all/CMakeLists.txt)

SET(ncvoxel_HEADERS
ncvoxel.h
)
prepend(ncvoxel_HEADERS ./inc)

SET(ncvoxel_SOURCES
ncvoxel.cpp
)
prepend(ncvoxel_SOURCES ./src)

SET(ncvoxel_RESOURCES ./ncvoxel_shaders.qrc)

QT4_WRAP_CPP(ncvoxel_HEADERS_MOC ${ncvoxel_HEADERS})
QT4_ADD_RESOURCES(ncvoxel_RESOURCES_RCC ${ncvoxel_RESOURCES})

ADD_library(ncvoxel STATIC ${ncvoxel_SOURCES} ${ncvoxel_HEADERS} ${ncvoxel_HEADERS_MOC} ${ncvoxel_RESOURCES_RCC})


