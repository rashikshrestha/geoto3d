#ifndef __NC_FLOW_H__
#define __NC_FLOW_H__

#include "ncgl.h"
#include "nccalibration.h"
#include "ncteximage.h"
#include "ncqtprogram.h"
#include "ncqtconfig3D.h"
#include "ncundistort.h"
#include "ncmesh.h"

#include "ncfbo.h"
#include "ncpbo.h"
#include "ncmdl.h"

#ifndef WIN32
#include "ncvoxel.h"
#endif

#include <cuda.h>
#include <cudaGL.h>
#include <cuda_gl_interop.h>
#include <cuda_runtime_api.h>
//#include <cutil_inline.h>

class ncFlow : public QObject {

    Q_OBJECT

signals:
    void updateSignal();

public:
    ncFlow(ncQtConfig3D* cfg_);
    ~ncFlow();

    void initBpSweep();
    void exitBpSweep();
    void bpSweep();
    void initSweep();

    void run();
    //void checkDepths();

    void drawFSrect();

    void gaussC1(ncFBO* fbo, ncTexImage2D* src,ncTexImage2D* tmp, ncTexImage2D* dst, ncPBO* gfact, int lod);
    void gaussC1Sub(ncFBO* fbo, ncTexImage2D* src,ncTexImage2D* tmp, ncPBO* gfact, int lod);
    void gaussC2(ncFBO* fbo, ncTexImage2D* src,ncTexImage2D* tmp, ncTexImage2D* dst, ncPBO* gfact, int lod);
    void medianC2(ncTexImage2D* rg_depth, int range , float zrange);
    void grayscale(ncTexImage2D* src, ncTexImage2D* dst, int lod);
    void setupGauss(ncPBO* pbo, float sigma, bool even = false);

    int                         ref;
    std::vector<ncTexInfo>      texinfo;
    ncPtr<ncTexImage2D>         zTexFBO;

private:

    void setupPtexPBO(int ref);
    void setupZlutPBO(int ref);

    void runPass1(int cid);
    void sweepPass1();
    void allocateStoragePass1(int cam_id);
    void loadNextImagePass1();

    void runCD(int cid);
    void sweepCD(int cam_id);
    void allocateStorageCD(int cam_id);
    void loadNextImageCD();

    void runDEL(int cid);

    void runMDL(int cid);
    void sweepMDL();
    void allocateStorageMDL(int cam_id);
    void loadNextImageMDL();

    void clearStorage();

    ncQtConfig3D*               cfg;

    int                         nrim;
    int                         imw,imh;
    int                         lodw,lodh,imlod;
    int                         winw,winh,wind,winlod;
    int                         mdl_upscale;

    std::vector<ncCommand>      cmd_list;
    int                         cur_cmd;

    char                        mystr[1024];

    ncPtr<ncFBO>                fbo;
    std::vector<ncc::m4x4f>     Ptex;
    ncPtr<ncPBO>                PtexPBO;
    std::vector<float>     		Zlut;
    ncPtr<ncPBO>                ZlutPBO;
  	cudaGraphicsResource        *ZlutCuda;
    float                       *ZlutDev;

    ncPtr<ncTexImage2DArray>    cTex;
    ncPtr<ncTexImage2DArray>    zTex;

    ncPtr<ncTexImage2D>         loadTex;
    ncPtr<ncTexImage2D>         undistortTex;
    ncPtr<ncTexImage2D>         tmpTex[2];

    std::vector<ncc::v4d>       vvec;
    std::vector<ncc::v4ub>      cvec;

    char                        syscall[1024];

    ncPtr<ncPBO>                pbo3D0;
    ncPtr<ncPBO>                pbo3D1;
    ncPtr<ncPBO>                pbo3D2;
  	cudaGraphicsResource        *pbo3D0cuda,*pbo3D1cuda,*pbo3D2cuda;
    float                       *idata0,*idata1,*idata2;
    float2                      *cdata;



    cudaGraphicsResource*       zTexFBO_resource;
    cudaArray*                  zTexFBO_array;

    static int                  refcount;
    static ncPtr<ncProgram>     grayscale_prog;
    static ncPtr<ncProgram>     gauss_c1_X;
    static ncPtr<ncProgram>     gauss_c1_Y;
    static ncPtr<ncProgram>     gauss_c2_X;
    static ncPtr<ncProgram>     gauss_c2_Y;
    static ncPtr<ncProgram>     sub_X;
    static ncPtr<ncProgram>     sub_Y;
    static ncPtr<ncProgram>     delaunay_prog;
    static ncPtr<ncProgram>     edge_prog;
    static ncPtr<ncProgram>     draw_rg_prog;
    static ncPtr<ncProgram>     mesh_prog;
    static ncPtr<ncProgram>     gvm_prog;
    static ncPtr<ncProgram>     bp_sweep;

    static ncPtr<ncProgram>     laplace_prog;
    static ncPtr<ncProgram>     carve_prog;
    static ncPtr<ncProgram>     cd_sweep;
    static ncPtr<ncProgram>     mdl_sweep;
    static ncPtr<ncPBO>         gFactPBO;
    static ncPtr<ncPBO>         gFactPBO_RG;
    static float                Gsigma;
};

#endif //__NC_FLOW_H__
