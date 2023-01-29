PROJECT(nccodec)

cmake_minimum_required( VERSION 2.6 )
INCLUDE(../all/CMakeLists.txt)

SET(nccodec_HEADERS inc/nccodec.h)

SET(nccodec_SOURCES src/nccodec.cpp )

ADD_LIBRARY(nccodec ${nccodec_HEADERS} ${nccodec_SOURCES})


