#ifndef __NC__CARVE_H__
#define __NC__CARVE_H__

#include "ncgl.h"
#include "ncgvm.h"
#include "ncqtconfig3D.h"
#include <deque>

class ncCarve  : public QObject {

    Q_OBJECT

public:
    ncCarve(ncVoxTree*  vtree_ , ncQtConfig3D* cfg_);
    ncCarve(ncGVM*      gvm_   , ncQtConfig3D* cfg_);
    ~ncCarve();
    void        init();

    void        run();
    void        process(ncTexImage2D* im, ncc::m4x4d KGL, ncc::RTd MV , double roi);
    void        update(ncSubVoxTree* vt);

    void        getMesh(int levelT_);

    void        createColors();
    void        writePLY(const char* filename);

signals:
    void        updateSignal();

private:

    bool        getValue(unsigned int& vmask, ncc::v4i pos);
    bool        getValue(unsigned int voxid, unsigned int& vmask, ncc::v4i pos, int& level_);

    void        getMesh(unsigned int voxid, ncc::v4i   pos     , const char* filename    , int levelT_);
    void        updateMesh(unsigned int voxid, ncc::v4i pos);
    void        optimizeMesh();

    void        addVox(ncc::v4i pos, unsigned int locmask);
    void        addSmallVox(ncc::v4i pos, unsigned int locmask);
    void        addLargeVox(ncc::v4i pos, unsigned int locmask);

    ncc::v3f    pos2vert(ncc::v4i pos);
    void        createColors(ncTexImage2D* cTex, ncc::m4x4d KGL , ncc::RTd MV , double zfar);

private:

    ncVoxTree*     vtree;
    ncGVM*         gvm;

    ncPBO*                  pPBO;
    ncPBO*                  vPBO;
    std::vector<int>        vindex;

    int            levelT;

    std::vector<ncc::v3f4ub>    mesh_vert;
    std::vector<unsigned int>   mesh_index;
    std::vector<ncc::v4ub>      mesh_color;

    std::vector<ncVoxTreeIterator>*      proc_src;
    std::vector<ncVoxTreeIterator>*      proc_dst;

    ncQtConfig3D*     cfg;
    char            mystr[1024];

    static int          refcount;
    static ncProgram*   vox_prog;
    static ncProgram*   color_prog;
};

#endif
