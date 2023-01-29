#include "ncmesh.h"

ncIndexArray::ncIndexArray() {
    m_vbo_index		= new ncPBO();
    nrindex			= 0;
}

ncIndexArray::~ncIndexArray() {
    clear();
    delete m_vbo_index;
}

void ncIndexArray::clear() {
    nrindex = 0;
}

void ncIndexArray::setup(int nrind, unsigned int* idata) {

    clear();

    nrindex = nrind;
	m_vbo_index->setup( nrindex, idata);
}

void ncIndexArray::setup(int nrind, ncc::v3ui* idata) {

    clear();
    nrindex = nrind*3;
	m_vbo_index->setup( nrind, &idata[0]);
}

GLuint ncIndexArray::getID() {
    return m_vbo_index->getID();
}

GLuint ncIndexArray::getNrIndices() {
    return nrindex;
}

ncIndexArray2D::ncIndexArray2D() {
}

ncIndexArray2D::~ncIndexArray2D() {
    clear();
}

void ncIndexArray2D::setSize(int w, int h) {
	width 	= w;
    height 	= h;

    unsigned int* idata = new unsigned int[(height-1)*(2*width+1)];
    for (int y=0; y<height-1; ++y)
    {
    	for (int x=0; x<width; ++x) {
            idata[y*(2*width+1)+2*x+0] =  y	*width+x;
            idata[y*(2*width+1)+2*x+1] = (y+1)*width+x;
        }
        idata[y*(2*width+1)+2*width] = 0xFFFFFFFF;
     }

	setup((height-1)*(2*width+1),idata);
	delete[] idata;
}

int ncIndexArray2D::getWidth() {
    return width;
}

int ncIndexArray2D::getHeight() {
    return height;
}

ncMesh::ncMesh() {
    glGenVertexArrays(1, &id);
    clear();
}

ncMesh::~ncMesh() {
    glDeleteVertexArrays(1, &id);
    clear();
}

void ncMesh::clear() {
	//for (int i=0;i<16;++i)
	//	m_pbo[i] = 0;
}

void ncMesh::draw(GLenum prim, int nrelements_, int offset) {

    //enableArrays();
    glBindVertexArray(id);
    glDrawArrays(prim,offset, nrelements_);
    //disableArrays();
	glBindVertexArray(0);
   }

void ncMesh::draw(GLenum prim,ncIndexArray* indices) {

    //enableArrays();
	glBindVertexArray(id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->getID());
    glDrawElements(prim, indices->getNrIndices(), GL_UNSIGNED_INT, 0 );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );

    //disableArrays();
	glBindVertexArray(0);

}

void ncMesh::draw(GLenum prim,ncIndexArray* indices, int nrelements_, int offset) {

    //enableArrays();
	glBindVertexArray(id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->getID());
    glDrawElements(prim, nrelements_, GL_UNSIGNED_INT, BUFFER_OFFSET(offset*sizeof(int)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );

    //disableArrays();
	glBindVertexArray(0);
}


void ncMesh::draw(GLenum prim,ncIndexArray2D* indices) {

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFFFFFF);

    //enableArrays();
	glBindVertexArray(id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->getID());
    glDrawElements(prim, indices->getNrIndices(), GL_UNSIGNED_INT, 0 );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );

    //disableArrays();
	glBindVertexArray(0);

    glDisable(GL_PRIMITIVE_RESTART);
}

void ncMesh::bindAttribF(int location, ncPBO* pbo, int size, GLenum type, bool normalized, int stride, int offset ) {
	//m_pbo[location] = pbo;

    glBindVertexArray(id);
    glBindBuffer(GL_ARRAY_BUFFER, pbo->getID() );
    glVertexAttribPointer((GLuint)location, size, type, normalized, stride, BUFFER_OFFSET(offset));
    glBindBuffer(GL_ARRAY_BUFFER, 0 );
	glEnableVertexAttribArray(location);
    glBindVertexArray(0);
}

void ncMesh::bindAttribI(int location, ncPBO* pbo, int size, GLenum type, int stride, int offset ) {
	//m_pbo[location] = pbo;

    glBindVertexArray(id);
    glBindBuffer(GL_ARRAY_BUFFER, pbo->getID() );
    glVertexAttribIPointer((GLuint)location, size, type, stride, BUFFER_OFFSET(offset));
    glBindBuffer(GL_ARRAY_BUFFER, 0 );
	glEnableVertexAttribArray(location);
    glBindVertexArray(0);
}
/*
void ncMesh::enableArrays() {

	glBindVertexArray(id);
    for (int i=0;i<16;++i)
		if (m_pbo[i])
			glEnableVertexAttribArray(i);
}

void ncMesh::disableArrays() {
	for (int i=0;i<16;++i)
		if (m_pbo[i])
			glDisableVertexAttribArray(i);
    glBindVertexArray(0);
}
*/
