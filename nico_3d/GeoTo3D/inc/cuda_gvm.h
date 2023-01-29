#include <vector_functions.h>
#include "cuda_common.h"

struct  __align__(8)  cudaVox {
    unsigned int   child;
    unsigned int   carve;
};

struct __align__(16) cudaGVM {
    cuda_m4x4       MV;
    cuda_m4x4       Ki;
    cuda_m4x4       MVP;
    float3          bpos;
    float3          bsize;
    int             maxlevel;
    int             lod;
    cudaVox*        voxels;
};
