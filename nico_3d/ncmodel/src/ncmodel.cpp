#include "ncmodel.h"

#include "nccommon.h"

#include "ncmdl.h"
#include "ncassimp.h"

#ifndef WIN32
#include "ncgvm.h"
#endif

ncModel::ncModel() : valid(false) {

}

ncModel::~ncModel() {
    clear();
}

ncModel* ncModel::loadFile(std::string fname) {

    std::string extension = ncc::getFileExt(fname);

#ifndef WIN32
	if (!strcmp(extension.c_str(),"mdl"))
        return new ncMDL(fname);
    else if (!strcmp(extension.c_str(),"gvm"))
        return new ncGVM(fname);
    else
        return new ncAssimp(fname);
#else
	if (!strcmp(extension.c_str(),"mdl"))
        return new ncMDL(fname);
    else
        return new ncAssimp(fname);
#endif
}

bool ncModel::load(std::string fname) {
    clear();
    if (!fname.empty() && ncc::checkFile(fname.c_str()) && v_load(fname))
    {
        valid       = true;
        filename    = fname;
    }
	return true;
}

void ncModel::clear() {
    if (valid) {
        valid = false;
        filename.clear();
    }
}

void ncModel::draw(ncc::m4x4d KGL,ncc::RTd MVi) {
    if (valid)
        v_draw(KGL,MVi);
}
