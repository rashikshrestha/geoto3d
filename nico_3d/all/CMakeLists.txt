cmake_policy(PUSH)

#set ( CMAKE_BUILD_TYPE Release )

find_package(OpenGL REQUIRED )
#find_package(NVQt4  QUIET )
find_package(Qt4    REQUIRED )

SET(QT_USE_QTOPENGL TRUE )
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

###############################################################################
# define some macros
macro(add_perforce_to_target _target)
  set_target_properties(${_target} PROPERTIES
    VS_SCC_PROJECTNAME "Perforce Project"
    VS_SCC_LOCALPATH "${CMAKE_SOURCE_DIR}"
    VS_SCC_PROVIDER "MSSCCI:Perforce SCM"
  )
endmacro()

###############################################################################

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CUDA_VERBOSE_BUILD ON)
find_package(CUDA REQUIRED)

set(CUDA_NVCC_FLAGS -arch=sm_30)
set (CUDA_HEADERS /usr/local/cuda/include )

CUDA_INCLUDE_DIRECTORIES ( ${CUDA_HEADERS} )
INCLUDE_DIRECTORIES( ${CUDA_HEADERS} )


SET(CMAKE_CXX_FLAGS "-DLINUX -D__STDC_CONSTANT_MACROS -Wfatal-errors -ffriend-injection -fpermissive")

INCLUDE_DIRECTORIES(
/usr/include/ImageMagick
${CMAKE_CURRENT_SOURCE_DIR}/../../nico_external/assimp--3.0.1270/include
#${CMAKE_CURRENT_SOURCE_DIR}/../../nico_external/ffmpeg/build/include
/usr/include/ffmpeg
)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/../../nico_external/libpgf/include
${CMAKE_CURRENT_SOURCE_DIR}/../
${CMAKE_CURRENT_SOURCE_DIR}/./
${CMAKE_CURRENT_SOURCE_DIR}/./inc
${CMAKE_CURRENT_SOURCE_DIR}/./include
${CMAKE_CURRENT_SOURCE_DIR}/../libpgf/include
${CMAKE_CURRENT_SOURCE_DIR}/../nccodec/inc
${CMAKE_CURRENT_SOURCE_DIR}/../nccommon/include
${CMAKE_CURRENT_SOURCE_DIR}/../ncqt/include
${CMAKE_CURRENT_SOURCE_DIR}/../ncqt/style/include
${CMAKE_CURRENT_SOURCE_DIR}/../ncvoxel/inc
${CMAKE_CURRENT_SOURCE_DIR}/../nccarve/inc
${CMAKE_CURRENT_SOURCE_DIR}/../ncflow/inc
)

LINK_DIRECTORIES(
/usr/local/lib
/usr/local/lib64
${CMAKE_CURRENT_SOURCE_DIR}/../../nico_external/libpgf/lib
${CMAKE_CURRENT_SOURCE_DIR}/../../nico_external/assimp--3.0.1270/build/code
#${CMAKE_CURRENT_SOURCE_DIR}/../../nico_external/ffmpeg/build/lib
${CMAKE_CURRENT_SOURCE_DIR}/../nccodec/build/lib
${CMAKE_CURRENT_SOURCE_DIR}/../nccommon/build/lib
${CMAKE_CURRENT_SOURCE_DIR}/../ncpromesh/build/lib
${CMAKE_CURRENT_SOURCE_DIR}/../ncqt/build/lib
${CMAKE_CURRENT_SOURCE_DIR}/../ncvoxel/build/lib
${CMAKE_CURRENT_SOURCE_DIR}/../ncmodel/build/lib
${CMAKE_CURRENT_SOURCE_DIR}/../nccarve/build/lib
${CMAKE_CURRENT_SOURCE_DIR}/../ncflow/build/lib
)

SET(COMMON_LIBS_ARCH Magick++ MagickCore MagickWand GLU GL pgf GLEW swscale avformat avcodec avutil x264 vpx va pthread bz2 z)
SET(COMMON_LIBS ${COMMON_LIBS_ARCH} ${QT_LIBRARIES} ${CUDA_LIBRARIES} )

macro(prepend PP_VAR PP_PATH)
	set(TMP_LIST)
    foreach(f ${${PP_VAR}})
        LIST(APPEND TMP_LIST ${PP_PATH}/${f})
    endforeach(f)
	SET( ${PP_VAR} ${TMP_LIST} )
endmacro(prepend)

cmake_policy(POP)
