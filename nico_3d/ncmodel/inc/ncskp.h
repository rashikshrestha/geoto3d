#ifndef __NC_SKP_H__
#define __NC_SKP_H__

#include <string>
#include "ncmesh.h"
#include "ncmodel.h"
#include "ncpbo.h"
#include "ncqtprogram.h"

#include <slapi/slapi.h>
#include <slapi/geometry.h>
#include <slapi/initialize.h>
#include <slapi/model/model.h>
#include <slapi/model/entities.h>
#include <slapi/model/group.h>
#include <slapi/model/face.h>
#include <slapi/model/edge.h>
#include <slapi/model/vertex.h>
#include <slapi/model/mesh_helper.h>
#include <slapi/model/uv_helper.h>
#include <slapi/model/geometry_input.h>
#include <slapi/model/material.h>
#include <slapi/model/texture.h>
#include <vector>

struct ncSKPface {

	ncSKPface(unsigned int tri_start, unsigned int tri_count) : tri_start(tri_start) , tri_count(tri_count) {};

	unsigned int tri_start;
	unsigned int tri_count;
};

class ncSKPd : public ncModel {

public:
	ncSKPd();
	ncSKPd(SUEntitiesRef & entities , ncc::RTd transform_);
    ~ncSKPd();

    void clear();
    void draw(ncc::m4x4d KGL , ncc::RTd MVi);

//private:
    void loadEntities(SUEntitiesRef & entities , ncc::RTd transform_);
	ncc::RTd convert(SUTransformation su_transform);

	ncPtr<ncMesh>               mesh;
    ncPtr<ncPBO >               d_vert;
    ncPtr<ncIndexArray >        d_ind;
    std::vector<ncc::v3f>		h_vert;
    std::vector<ncc::v3ui>		h_ind;
	ncc::v3f					pmin;
	ncc::v3f					pmax;

	std::vector<ncSKPface>		faces;
	std::vector<ncSKPd*>		groups;

	ncc::RTd					transform;

};

class ncSKP : public ncSKPd {

public:
    ncSKP(std::string fname);
    ~ncSKP();

    void clear();
    void load(std::string fname);
    void save(std::string fname);

//private:
    std::string					filename;
    static int                  refcount;
    static ncPtr<ncQtProgram>   skp_prog;

};

#endif
