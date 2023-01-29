#ifndef NCMESH_H
#define NCMESH_H

//#include <GL/glew.h>
#include "ncmath.h"
#include "ncpbo.h"

class ncIndexArray {

public:
    ncIndexArray();
    ~ncIndexArray();

    void 	clear();
    void 	setup(int nrind, unsigned int* idata);
    void 	setup(int nrind, ncc::v3ui*    idata);
    GLuint 	getID();
    GLuint 	getNrIndices();

public:
	ncPBO*	m_vbo_index;
    GLuint nrindex;
};

class ncIndexArray2D : public ncIndexArray {

public:
    ncIndexArray2D();
    ~ncIndexArray2D();

	void setSize(int w, int h);

    int getWidth();
    int getHeight();

private:
	int width;
	int height;
};

class ncMesh {

public:
    ncMesh();
    ~ncMesh();

    void 	clear();

    void 	draw(GLenum prim, int nrelements_, int offset = 0);
    void 	draw(GLenum prim,ncIndexArray  * indices);
	void	draw(GLenum prim,ncIndexArray  * indices, int nrelements_, int offset = 0);
	void 	draw(GLenum prim,ncIndexArray2D* indices);

    void 	bindAttribF(int location, ncPBO* pbo, int size, GLenum type, bool normalized, int stride = 0, int offset = 0);
    void 	bindAttribI(int location, ncPBO* pbo, int size, GLenum type, int stride = 0, int offset = 0);

//private:
//	void 	enableArrays();
//	void 	disableArrays();


//    ncPBO*						m_pbo[16];
    GLuint                      id;
};


#endif //NCMESH_H
