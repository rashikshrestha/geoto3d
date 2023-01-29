#ifndef __NC_MDL_H__
#define __NC_MDL_H__

#include <string>
#include "ncmesh.h"
#include "ncpbo.h"
#include "ncmodel.h"

class ncQtProgram;
class ncModel;

class ncMDL : public ncModel {

public:
    ncMDL();
    ncMDL(std::string fname);
    ~ncMDL();

    bool v_load(std::string fname);
    void v_draw(ncc::m4x4d KGL , ncc::RTd MVi);

    void init();
    void clear();
    void drawOffset(ncc::m4x4d KGL , ncc::RTd MVi , float depth_offset);

    static void save(std::string fname , ncc::RTd  ptransform , std::vector<ncc::v3f4ub>& pdata);
    static void load(std::string fname , ncc::RTd& ptransform , std::vector<ncc::v3f4ub>& pdata);

//private:
    void cpu2gpu();

    ncPtr<ncMesh>               mesh;
    ncPtr<ncPBO >               points;
    ncc::RTd                    transform;
    std::vector<ncc::v3f4ub>    data;

    static int                  refcount;
    static ncQtProgram          *mdl_prog;

};

#endif
