PROJECT(GeoTo3D)

cmake_minimum_required( VERSION 2.6 )
INCLUDE(../all/CMakeLists.txt)

INCLUDE_DIRECTORIES( ../ncmodel/inc)

CUDA_COMPILE( generated_files src/cuda_gvm.cu src/cuda_bp.cu )

LINK_DIRECTORIES()

if(WIN32)
SET(GeoTo3D_HEADERS inc/mainwindow.h inc/ncapp.h inc/ncqtconfig3D.h inc/ncflow.h inc/cuda_gvm.h inc/cuda_common.h inc/cuda_bp.h)
SET(GeoTo3D_SOURCES src/main.cpp src/mainwindow.cpp src/ncapp.cpp src/ncqtconfig3D.cpp src/ncflow.cpp ${GeoTo3D_HEADERS})
else()
SET(GeoTo3D_HEADERS inc/mainwindow.h inc/ncapp.h inc/ncqtconfig3D.h inc/ncflow.h inc/nccarve.h inc/cuda_gvm.h inc/cuda_common.h inc/cuda_bp.h)
SET(GeoTo3D_SOURCES src/main.cpp src/mainwindow.cpp src/ncapp.cpp src/ncqtconfig3D.cpp src/ncflow.cpp src/nccarve.cpp ${GeoTo3D_HEADERS})
endif()

SET(GeoTo3D_RESOURCES ./GeoTo3D_shaders.qrc)

QT4_WRAP_CPP(GeoTo3D_HEADERS_MOC ${GeoTo3D_HEADERS})
QT4_ADD_RESOURCES(GeoTo3D_RESOURCES_RCC ${GeoTo3D_RESOURCES})

add_definitions(-DAPP_NAME="GeoTo3D")
add_definitions(-DAPP_VERSION="1.0")

ADD_EXECUTABLE(GeoTo3D ${GeoTo3D_SOURCES} ${GeoTo3D_HEADERS_MOC} ${GeoTo3D_RESOURCES_RCC} ${generated_files})
TARGET_LINK_LIBRARIES(GeoTo3D ncvoxel ncmodel ncqt nccommon assimp ${COMMON_LIBS})
