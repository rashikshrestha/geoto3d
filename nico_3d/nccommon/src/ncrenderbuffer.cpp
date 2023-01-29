#include "ncrenderbuffer.h"

ncRenderBuffer::ncRenderBuffer(int iformat,int w,int h) {
	glGenRenderbuffers(1,&id);
	target 	= GL_RENDERBUFFER;
    setFormat(iformat);
	resize(w,h);
}

ncRenderBuffer::~ncRenderBuffer() {
	glDeleteRenderbuffers(1,&id);
}

void ncRenderBuffer::bind() const {
	glBindRenderbuffer(target,id);
}

void ncRenderBuffer::setFormat(GLenum iformat) {

    internalformat = iformat;

    switch (iformat)
    {
        //! 8 bit
        case GL_R8:
            format 			= GL_RED;
            type 			= GL_UNSIGNED_BYTE;
            bpp             = 1;
            break;
        case GL_RGB8:
            format 			= GL_RGB;
            type 			= GL_UNSIGNED_BYTE;
            bpp             = 3;
            break;
        case GL_RGBA8:
            format 			= GL_RGBA;
            type 			= GL_UNSIGNED_BYTE;
            bpp             = 4;
            break;

        //! 16 bit

        case GL_R16:
            format 			= GL_RED;
            type 			= GL_UNSIGNED_SHORT;
            bpp             = 2;
            break;

        case GL_RGB16:
            format 			= GL_RGB;
            type 			= GL_UNSIGNED_SHORT;
            bpp             = 6;
            break;

        case GL_RGBA16:
            format 			= GL_RGBA;
            type 			= GL_UNSIGNED_SHORT;
            bpp             = 8;
            break;

        case GL_R16F:
            format 			= GL_RED;
            type 			= GL_HALF_FLOAT;
            bpp             = 2;
            break;

        case GL_RG16F:
            format 			= GL_RG;
            type 			= GL_HALF_FLOAT;
            bpp             = 4;
            break;

        case GL_DEPTH_COMPONENT16:
            format 			= GL_DEPTH_COMPONENT;
            type 			= GL_UNSIGNED_SHORT;
            bpp             = 2;
            break;

        //! 32 bit
        case GL_RGBA32F:
            format 			= GL_RGBA;
            type 			= GL_FLOAT;
            bpp             = 16;
            break;

        case GL_RGBA32I:
            format 			= GL_RGBA32I;
            type 			= GL_INT;
            bpp             = 16;
            break;

        case GL_RGBA32UI:
            format 			= GL_RGBA32UI;
            type 			= GL_UNSIGNED_INT;
            bpp             = 16;
            break;

        case GL_R32I:
            format 			= GL_RED_INTEGER;
            type 			= GL_INT;
            bpp             = 4;
            break;

        case GL_R32F:
            format 			= GL_RED;
            type 			= GL_FLOAT;
            bpp             = 4;
            break;

        case GL_RG32F:
            format 			= GL_RG;
            type 			= GL_FLOAT;
            bpp             = 8;
            break;

        case GL_DEPTH_COMPONENT32:
            format 			= GL_DEPTH_COMPONENT;
            type 			= GL_UNSIGNED_INT;
            bpp             = 4;
            break;

        case GL_DEPTH_COMPONENT32F:
            format 			= GL_DEPTH_COMPONENT;
            type 			= GL_FLOAT;
            bpp             = 4;
            break;

        //! custom
        case GL_DEPTH24_STENCIL8:
            format          = GL_DEPTH_STENCIL;
            type            = GL_UNSIGNED_INT_24_8;
            bpp             = 4;
            break;
        default:
            fprintf(stderr,"Error: Unknown renderbuffer format %x\n",iformat);
            break;
    }

}

void ncRenderBuffer::resize(int width_, int height_) {

    if ((width!=width_) || (height!=height_))
	{
		width   = width_;
		height  = height_;

		glBindRenderbuffer(GL_RENDERBUFFER,id);
		glRenderbufferStorage(target,internalformat,width,height);
	}
}
