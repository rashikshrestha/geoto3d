PROJECT(GeoToView)

cmake_minimum_required( VERSION 2.8 )
INCLUDE(../all/CMakeLists.txt)

add_definitions(-DAPP_NAME="GeoToView")
add_definitions(-DAPP_VERSION="1.1")

INCLUDE_DIRECTORIES(
    ../ncmodel/inc
    /usr/include/eigen3
    /usr/include/pcl-1.7
)

LINK_DIRECTORIES(
)

SET(GeoToView_HEADERS inc/mainwindow.h  inc/ncapp.h)

SET(GeoToView_SOURCES src/main.cpp  src/mainwindow.cpp  src/ncapp.cpp ${GeoToView_HEADERS} )

SET(GeoToView_RESOURCES ./geotoview.qrc)

if(WIN32)
SET(GeoToView_LIBS ${COMMON_LIBS} ncqt ncmodel nccommon nccodec assimp )
else()
SET(GeoToView_LIBS ncqt ncmodel ncvoxel nccommon nccodec assimp pcl_octree pcl_kdtree pcl_filters pcl_common boost_system ${COMMON_LIBS})
#SET(GeoToView_LIBS ncqt ncmodel ncvoxel nccommon nccodec assimp ${COMMON_LIBS})
endif()

QT4_WRAP_CPP(GeoToView_HEADERS_MOC ${GeoToView_HEADERS})
QT4_ADD_RESOURCES(GeoToView_RESOURCES_RCC ${GeoToView_RESOURCES})

ADD_EXECUTABLE(GeoToView ${GeoToView_SOURCES} ${GeoToView_HEADERS_MOC} ${GeoToView_RESOURCES_RCC})
TARGET_LINK_LIBRARIES(GeoToView ${GeoToView_LIBS})
