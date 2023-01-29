FROM ubuntu:14.04

#! Dependencies
RUN apt update

# Auto time for cmake
ENV TZ=America/Los_Angeles
RUN ln -snf "/usr/share/zoneinfo/$TZ" /etc/localtime
RUN echo "$TZ" > /etc/timezone
RUN apt-get install -y tzdata

RUN apt install -y vim wget make g++ cmake unzip
RUN apt install -y libtool
RUN apt install -y software-properties-common

RUN apt update

RUN apt install -y automake

#! Copy code
COPY nico_3d geoto3d/nico_3d
COPY nico_external geoto3d/nico_external

#! Build and Install libpgf
WORKDIR /geoto3d/nico_external
RUN tar -xvvf libpgf-6.12.24-src.tar.gz
WORKDIR /geoto3d/nico_external/libpgf
RUN ./autogen.sh
RUN ./configure
RUN make
RUN make install


#! Install Boost
#TODO
# WORKDIR /geoto3d/nico_external
# RUN unzip boost-1.53.0.zip
# WORKDIR /geoto3d/nico_external/boost-1.53.0

#! Install Assimp
WORKDIR /geoto3d/nico_external
RUN unzip assimp-3.0.1270.zip
WORKDIR /geoto3d/nico_external/assimp-3.0.1270
RUN mkdir build
WORKDIR /geoto3d/nico_external/assimp-3.0.1270/build
RUN cmake -DENABLE_BOOST_WORKAROUND=ON ..
RUN make
RUN make install

#! Dependencies
RUN apt install -y qt4-default
RUN apt-get install -y nvidia-cuda-toolkit
RUN apt-get install -y libglew-dev
RUN apt-get install -y libmagick++-dev

#! Build ncvoxel nccommon ncqt ncmodel
WORKDIR /geoto3d/nico_external/ncvoxel
RUN mkdir build
WORKDIR /geoto3d/nico_external/ncvoxel/build
RUN cmake ..
RUN make

WORKDIR /geoto3d/nico_external/nccommon
RUN mkdir build
WORKDIR /geoto3d/nico_external/nccommon/build
RUN cmake ..
RUN make

WORKDIR /geoto3d/nico_external/ncqt
RUN mkdir build
WORKDIR /geoto3d/nico_external/ncqt/build
RUN cmake ..
RUN make

WORKDIR /geoto3d/nico_external/ncmodel
RUN mkdir build
WORKDIR /geoto3d/nico_external/ncmodel/build
RUN cmake ..
RUN make

#! Dependencies
RUN apt install -y libswscale-dev libavformat-dev libx264-dev libvpx-dev libva-dev

#! Install GeoTo3D
WORKDIR /geoto3d/nico_3d/GeoTo3D
RUN mkdir build
WORKDIR /geoto3d/nico_3d/GeoTo3D/build
RUN cmake ..
RUN make

# RUN add-apt-repository ppa:rock-core/qt4
# RUN apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
