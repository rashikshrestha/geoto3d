#include "ncdata.h"

ncCalibrationSet*    ncData::calset     = 0;
ncPoseList*          ncData::poselist   = 0;
ncMesh*              ncData::mesh       = 0;
ncIndexArray*        ncData::triangles  = 0;
char                 ncData::imagefiles[1024];
char                 ncData::cubefiles[1024];
int                  ncData::imw = 0;
int                  ncData::imh = 0;
