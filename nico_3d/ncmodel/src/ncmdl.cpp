#include "ncmdl.h"

#include "ncqtprogram.h"

int             ncMDL::refcount = 0;
ncQtProgram*    ncMDL::mdl_prog;

ncMDL::ncMDL() {
    init();
}

ncMDL::ncMDL(std::string fname) {
    init();
    ncModel::load(fname);
}

ncMDL::~ncMDL() {
    if (--refcount == 0)
    {
        delete mdl_prog;
        mdl_prog = 0;
    }
}

void ncMDL::init() {

    type = NC_MDL;

    if (refcount++ == 0)
    {
        Q_INIT_RESOURCE(ncmodel_shaders);
        mdl_prog = new ncQtProgram(":/shaders/mdl.vert",0,":/shaders/mdl.frag");
    }
}

void ncMDL::clear() {
    mesh.reset();
    points.reset();
    transform.setIdentity();
    data.clear();
    ncModel::clear();
}

bool ncMDL::v_load(std::string fname) {
    load(fname,transform,data);
    cpu2gpu();
    return true;
}

void ncMDL::v_draw(ncc::m4x4d KGL , ncc::RTd MVi) {
	drawOffset(KGL , MVi , 0.0f);
}

void ncMDL::cpu2gpu() {

    points = new ncPBO;
    points->setup(data.size(),&data[0]);

    mesh = new ncMesh;
    mesh->bindAttribF(0,points,3,GL_FLOAT        ,GL_FALSE, sizeof(ncc::v3f4ub),0                  );
    mesh->bindAttribF(1,points,4,GL_UNSIGNED_BYTE,GL_TRUE , sizeof(ncc::v3f4ub),sizeof(ncc::v3f)   );

}

void ncMDL::save(std::string fname , ncc::RTd ptransform , std::vector<ncc::v3f4ub>& pdata) {

    unsigned int file_type = 0;
    sprintf((char*)&file_type,"MDL");
    unsigned int file_version = 102;

    std::ofstream file(fname.c_str());
    //file.write((char*)&file_type        , sizeof(unsigned int           ));
    //file.write((char*)&file_version     , sizeof(unsigned int           ));

    file.write((char*)&ptransform[0][0] , sizeof(ncc::RTd               ));
    unsigned int nrpoints = pdata.size();
    file.write((char*)&nrpoints         , sizeof(unsigned int           ));
    file.write((char*)&pdata[0].x       , nrpoints*sizeof(ncc::v3f4ub   ));
    file.close();
}

void ncMDL::load(std::string fname , ncc::RTd& ptransform , std::vector<ncc::v3f4ub>& pdata) {

    unsigned int file_type ,file_version;

	std::ifstream file(fname.c_str(),std::ifstream::binary);
    //file.read((char*)&file_type        , sizeof(unsigned int           ));
    //file.read((char*)&file_version     , sizeof(unsigned int           ));
    file.read((char*)&ptransform[0][0] , sizeof(ncc::RTd               ));
    unsigned int nrpoints;
    file.read((char*)&nrpoints         , sizeof(unsigned int           ));
    pdata.resize(nrpoints);
    file.read((char*)&pdata[0].x       , nrpoints*sizeof(ncc::v3f4ub   ));
    file.close();
}

void ncMDL::drawOffset(ncc::m4x4d KGL , ncc::RTd MVi , float depth_offset) {

    ncc::m4x4d MVP = KGL*MVi*transform;

    mdl_prog->Activate();
    mdl_prog->setUniform("MVP",MVP);
    mdl_prog->setUniform("depth_offset",depth_offset);

    mesh->draw(GL_POINTS,points->nrelements);

    mdl_prog->Deactivate();
}
