#ifndef __NC_ASSIMP_H__
#define __NC_ASSIMP_H__

#include <string>
#include "ncgl.h"
#include "ncmesh.h"
#include "ncmodel.h"
#include "ncpbo.h"
#include "ncqtprogram.h"


//#include <assimp.hpp>
//#include <aiScene.h>
//#include <aiPostProcess.h>

#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

struct ncAssimpMesh {

    ncAssimpMesh(const aiMesh* paiMesh);
    ~ncAssimpMesh();

    GLuint vao;
    GLuint v_pbo;
    GLuint c_pbo;
    GLuint t_pbo;
    GLuint i_pbo;

    std::vector<ncc::v3f>   vertices;
    std::vector<ncc::v2f>   texcoords;
    std::vector<ncc::v4f>   colors;
    std::vector<ncc::v3i>   indices;

    bool                    hasTexCoords;
    bool                    hasColors;

    unsigned int            materialIndex;
};

struct ncAssimpMaterial {

    ncAssimpMaterial() : tex(0) {};
    ~ncAssimpMaterial() {
        if (tex)
        {
            delete tex;
            tex = 0;
        }
    };

    ncTexImage2D*   tex;
};

class ncAssimp : public ncModel {

public:
    ncAssimp();
    ncAssimp(std::string fname);
    ~ncAssimp();

    bool v_load(std::string fname);
    void v_draw(ncc::m4x4d KGL , ncc::RTd MVi);

    void init();
    void clear();
    void recursive_render (const struct aiScene *sc, const struct aiNode* nd , ncc::m4x4d KGL , ncc::RTd MVi);
    void collect( std::vector<ncc::v3f>& vert , std::vector<ncc::v3i>& ind);
    void recursive_collect( std::vector<ncc::v3f>& vert , std::vector<ncc::v3i>& ind , const struct aiScene *sc, const struct aiNode* nd , ncc::RTd MVi);

//private:
    ncc::RTd                        transform;

    static int                      refcount;
    static ncPtr<ncQtProgram>       assimp_prog;

    std::vector<ncAssimpMesh*>      meshes;
    std::vector<ncAssimpMaterial>   materials;
    const aiScene*                  pScene;
    Assimp::Importer                Importer;

};

#endif
