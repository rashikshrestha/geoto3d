#include "ncmodel.h"
#include "ncvoxel.h"

//! SubGVM
class ncSubGVM : public ncSubVoxTree {

public:
    ncSubGVM(std::string fname = "");
    ncSubGVM(std::string filename_ , unsigned int data_mask_ , ncc::v3ll pmin_ , int maxlevel_ , int minlevel_ , unsigned int maxcarve_);
    ~ncSubGVM();

    void                    draw(ncc::m4x4d KGL,ncc::RTd MV , GLenum ccv);
    void                    drawMesh(ncc::m4x4d KGL,ncc::RTd MV , GLenum ccv);
    void                    cpu2gpu();

protected:
    void                    init();
    void                    clear();
    bool                    load(std::string fname);

public:
    ncPtr<ncPBO>            child_pbo;
    ncPtr<ncPBO>            rgbm_pbo;
    ncPtr<ncPBO>            color_pbo;
    ncPtr<ncPBO>            vpbo;
    ncPtr<ncMesh>           mesh;

    static int              refcount;
    static ncPtr<ncProgram> render_prog;
    static int              lod;
};


//! ncGVM
class ncGVM : public ncVoxTree , public ncModel {

public:
    ncGVM();
    ncGVM(std::string fname);
    ~ncGVM();

    bool    v_load(std::string fname);
    void    v_draw(ncc::m4x4d KGL,ncc::RTd MVi);


    void    init();
    void    clear();
    void    setCameraPosition(ncc::m4x4d KGL , ncc::RTd MV , int w, int h , GLenum cw_dir);
    void    drawCCV(ncc::m4x4d KGL,ncc::RTd MV , GLenum ccv);
    void    loadSubLevel(ncVoxTreeIterator it);
    void    loadSubLevel(ncVoxTreeIterator it , unsigned int data_mask , ncc::v3ll pmin , int maxlevel , int minlevel , unsigned int maxcarve);
    void    saveSubLevel(ncVoxTreeIterator it);

    ncPtr<ncFBO>            fbo;
    ncPtr<ncTexImage2D>     dtex;

    ncPtr<ncPBO>            vpbo;
    ncPtr<ncMesh>           mesh;
    std::vector<GLuint>     occ;
    std::vector<int>        vis;

    static int              refcount;
    static ncPtr<ncProgram> render_prog;

};
