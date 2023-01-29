#ifndef __NC_DATA_H__
#define __NC_DATA_H__

#include "nccalibrationset.h"
#include "ncposelist.h"
#include "ncmesh.h"

class ncData {

public:
    static ncCalibrationSet*    calset;
    static ncPoseList*          poselist;
    static ncMesh*              mesh;
    static ncIndexArray*        triangles;
    static char                 imagefiles[1024];
    static char                 cubefiles[1024];
    static int                  imw,imh;


};

#endif
