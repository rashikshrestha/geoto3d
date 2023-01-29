PROJECT(GeoToLas)

cmake_minimum_required( VERSION 2.8 )
INCLUDE(../all/CMakeLists.txt)

add_definitions(-DAPP_NAME="GeoToLas")
add_definitions(-DAPP_VERSION="1.1")

INCLUDE_DIRECTORIES(
/usr/local/include
${CMAKE_CURRENT_SOURCE_DIR}/../nccommon/include
)

LINK_DIRECTORIES(
/usr/local/lib
)

SET(GeoToLas_HEADERS )

SET(GeoToLas_SOURCES src/main.cpp ${GeoToLas_HEADERS} )

SET(GeoToLas_LIBS las_c nccommon ${COMMON_LIBS} )

ADD_EXECUTABLE(GeoToLas ${GeoToLas_SOURCES} )
TARGET_LINK_LIBRARIES(GeoToLas ${GeoToLas_LIBS})
