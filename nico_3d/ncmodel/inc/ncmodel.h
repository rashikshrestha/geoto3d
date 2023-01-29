#ifndef __NC_MODEL_H__
#define __NC_MODEL_H__

#include <string>
#include "ncmath.h"

enum model_type { NC_MDL , NC_GVM , NC_ASSIMP };

class ncModel {

public:
    ncModel();
    virtual ~ncModel();

    static ncModel* loadFile(std::string fname);

    void    clear();
    bool    load(std::string fname);
    void    draw(ncc::m4x4d KGL,ncc::RTd MVi);

    virtual bool    v_load(std::string fname) = 0;
    virtual void    v_draw(ncc::m4x4d KGL,ncc::RTd MVi) = 0;

protected:
    bool            valid;
    std::string     filename;
    model_type      type;
};

#endif //__NC_MODEL_H__
