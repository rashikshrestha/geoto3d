#include "ncpbo.h"

ncPBO::ncPBO(GLenum type , GLenum mode) {

    init();
    setup_void(0,0, type , mode);
}

ncPBO::~ncPBO() {
    makeNonResident();
	glDeleteBuffers(1,&id);
}

void ncPBO::init() {
    size        = 0;
    nrelements  = 0;
    gpu_addr    = 0;
    gpu_mode    = 0;
    glGenBuffers(1,&id);

}

void ncPBO::bind(GLenum target) {
	glBindBuffer(target,id);
}

void ncPBO::unbind(GLenum target) {
	glBindBuffer(target,0);
}

GLuint ncPBO::getID() {
	return id;
}

void ncPBO::setup_void(int size_ , void* data , GLenum type, GLenum mode) {

    if (size_!=0)
    {
        size = size_;

        makeNonResident();
        bind(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER,size,data,type);
        unbind(GL_ARRAY_BUFFER);
        makeResident(mode);
    }
}

void ncPBO::makeResident(GLenum mode) {
    if (isResident())
    {
        if (mode!=gpu_mode)
        {
            makeNonResident();
            glMakeNamedBufferResidentNV(id,mode);
            glGetNamedBufferParameterui64vNV(id, GL_BUFFER_GPU_ADDRESS_NV, &gpu_addr);
            gpu_mode = mode;
        }
    }
    else
    {
        glMakeNamedBufferResidentNV(id,mode);
        glGetNamedBufferParameterui64vNV(id, GL_BUFFER_GPU_ADDRESS_NV, &gpu_addr);
        gpu_mode = mode;
    }
}

void ncPBO::makeNonResident() {
    if (isResident())
        glMakeNamedBufferNonResidentNV(id);
}

bool ncPBO::isResident() {
    return glIsNamedBufferResidentNV(id);
}

GLuint64EXT ncPBO::getGpuAddress() {
    return gpu_addr;
}
