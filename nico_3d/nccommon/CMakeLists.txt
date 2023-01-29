PROJECT(nccommon)

cmake_minimum_required( VERSION 2.6 )
INCLUDE(../all/CMakeLists.txt)

SET(nccommon_HEADERS
    master.h
    nccalibration.h
    nccalibrationset.h
    nccommon.h
    ncconfig.h
    ncdata.h
    ncfbo.h
    ncgl.h
    ncmath.h
    ncmesh.h
    ncpbo.h
#    ncpointmap.h
    ncposelist.h
    ncprogram.h
    ncrenderbuffer.h
    ncteximage.h
    nctrack.h
    nctracklist.h
    ncundistort.h
    tetgen.h
    triangle.h
)
prepend(nccommon_HEADERS ./include)

SET(nccommon_SOURCES
    nccalibration.cpp
    nccalibrationset.cpp
    nccommon.cpp
    ncdata.cpp
    ncfbo.cpp
    ncgl.cpp
    ncmesh.cpp
    ncpbo.cpp
#    ncpointmap.cpp
    ncposelist.cpp
    ncprogram.cpp
    ncundistort.cpp
    ncrenderbuffer.cpp
    ncteximage.cpp
    nctrack.cpp
    nctracklist.cpp
    predicates.cpp
    tetgen.cpp
    triangle.cpp
)
prepend(nccommon_SOURCES ./src)

ADD_LIBRARY(nccommon ${nccommon_HEADERS} ${nccommon_SOURCES} )


