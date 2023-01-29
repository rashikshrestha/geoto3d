#include "ncteximage.h"
#include "ncprogram.h"
#include <stdio.h>
#include <algorithm>

ncTexImage::ncTexImage() {
	glGenTextures(1,&id);
	bpp		=  0;
	width	= -1;
	height	= -1;
	depth	= -1;
}

ncTexImage::~ncTexImage() {
	glDeleteTextures(1,&id);

}

void ncTexImage::bind() const {
	glBindTexture(target,id);
}

void ncTexImage::bind(GLenum texunit) const {
	glActiveTexture(GL_TEXTURE0+texunit);
	bind();
}

void ncTexImage::setFilters(GLenum min, GLenum mag) {
	bind(0);
	glTexParameteri( target, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri( target, GL_TEXTURE_MIN_FILTER, min);
}

void ncTexImage::setWrapModes(GLenum wx, GLenum wy, GLenum wz) {
	bind(0);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wx);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wy);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, wz);
}

void ncTexImage::generateMipmap() {
	bind(0);
	glGenerateMipmap(target);
}

void ncTexImage::setFormat(GLenum iformat) {

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

        case GL_RGBA16F:
            format 			= GL_RGBA;
            type 			= GL_HALF_FLOAT;
            bpp             = 8;
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
            fprintf(stderr,"Error: Unknown texture format %x\n",iformat);
            break;
    }

}

ncTexImageRect::ncTexImageRect(int internalformat_,int width_,int height_) {

	depth 			= 1;
	target 			= GL_TEXTURE_RECTANGLE;
    setFormat(internalformat_);

	resize(width_,height_);

	setFilters(GL_NEAREST,GL_NEAREST);

	setWrapModes(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);

}

void ncTexImageRect::setData(void* data_) {

	glBindTexture(target,id);
    glTexSubImage2D(target,0,0,0,width,height,format,type,data_);
}

void ncTexImageRect::resize(int width_, int height_) {

    if ((width!=width_) || (height!=height_))
	{
		width   = width_;
		height  = height_;

		glBindTexture(target,id);
		glTexImage2D(target,0,internalformat,width,height,0,format,type,0);
	}
}


ncProgram* ncTexImage2D::draw_prog = 0;

ncTexImage2D::ncTexImage2D(int internalformat_,int width_,int height_) {

	target 			= GL_TEXTURE_2D;
    setFormat(internalformat_);
	resize(width_,height_);
	setFilters(GL_NEAREST,GL_NEAREST);
	setWrapModes(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
}

void ncTexImage2D::setData(void* data_) {

	glBindTexture(target,id);
    glTexSubImage2D(target,0,0,0,width,height,format,type,data_);
}

void ncTexImage2D::resize(int width_, int height_) {

	depth 	= 1;

	if ((width!=width_) || (height!=height_))
	{
		width   = width_;
		height  = height_;

		glBindTexture(target,id);
		glTexImage2D(target,0,internalformat,width,height,0,format,type,0);
	}
}

void ncTexImage2D::setAnisotropy(float aniso) {

    float max_aniso;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&max_aniso);
    aniso = (std::min)(max_aniso,(std::max)(1.0f,aniso));
    glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,aniso);
}

void ncTexImage2D::draw(ncc::m4x4f color_matrix , ncc::v4f color_offset , bool flip) {

    if (!draw_prog)
    {
        draw_prog = new ncProgram(
"#version 330 core                                  \n\
                                                    \n\
layout(location = 0) in		vec4 mPosition;         \n\
out		vec2 fTexCoord;                             \n\
                                                    \n\
void main(void)                                     \n\
{                                                   \n\
	gl_Position 	= vec4(mPosition.xy,0.0f,1.0f); \n\
	fTexCoord		= mPosition.zw;                 \n\
}"

,0,

"#version 330 core                                                  \n\
                                                                    \n\
uniform mat4 color_matrix;                                          \n\
uniform vec4 color_offset;                                          \n\
uniform sampler2D tex2D;                                            \n\
                                                                    \n\
in	vec2 fTexCoord;                                                 \n\
out vec4 fColor;                                                    \n\
                                                                    \n\
void main(void)                                                     \n\
{                                                                   \n\
	fColor = color_matrix*textureLod(tex2D,fTexCoord,0) + color_offset;  \n\
}"
, true);
    }

    draw_prog->Activate();
    draw_prog->setUniform("color_matrix",color_matrix);
    draw_prog->setUniform("color_offset",color_offset);
    draw_prog->setSampler("tex2D", this , 0);

    ncc::v4f tmp[4];

    if (flip)
    {
        tmp[0] = ncc::v4f(-1.0f,-1.0f,0.0f,1.0f);
        tmp[1] = ncc::v4f(+1.0f,-1.0f,1.0f,1.0f);
        tmp[2] = ncc::v4f(-1.0f,+1.0f,0.0f,0.0f);
        tmp[3] = ncc::v4f(+1.0f,+1.0f,1.0f,0.0f);
    }
    else
    {
        tmp[0] = ncc::v4f(-1.0f,-1.0f,0.0f,0.0f);
        tmp[1] = ncc::v4f(+1.0f,-1.0f,1.0f,0.0f);
        tmp[2] = ncc::v4f(-1.0f,+1.0f,0.0f,1.0f);
        tmp[3] = ncc::v4f(+1.0f,+1.0f,1.0f,1.0f);
    }

    glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, &tmp[0].x);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(0);

    draw_prog->Deactivate();
}




ncTexImageMultisample2D::ncTexImageMultisample2D(int internalformat_,int width_,int height_, int nrsamples_) {

	target 			= GL_TEXTURE_2D_MULTISAMPLE;
	nrsamples       = nrsamples_;

    setFormat(internalformat_);
	resize(width_,height_);
}

void ncTexImageMultisample2D::resize(int width_, int height_) {

	depth 	= 1;

	if ((width!=width_) || (height!=height_))
	{
		width   = width_;
		height  = height_;

		glBindTexture(target,id);
        glTextureImage2DMultisampleNV(id, target, nrsamples, internalformat, width,height, GL_TRUE);
	}
}




ncTexImage2DArray::ncTexImage2DArray(int internalformat_,int width_,int height_,int depth_) {

    target 			= GL_TEXTURE_2D_ARRAY;
    setFormat(internalformat_);
    resize(width_,height_,depth_);
	setFilters(GL_NEAREST,GL_NEAREST);
	setWrapModes(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
}

void ncTexImage2DArray::setData(int index , void* data_) {

    glBindTexture(target,id);
    glTexSubImage3D(target,0,0,0,index,width,height,1,format,type,data_);
}

void ncTexImage2DArray::resize(int width_, int height_, int depth_) {

	if ((width!=width_) || (height!=height_) || (depth!=depth_))
	{
		width   = width_;
		height  = height_;
		depth   = depth_;

		glBindTexture(target,id);
		glTexImage3D(target,0,internalformat,width,height,depth,0,format,type,0);
	}
}




ncTexImage3D::ncTexImage3D(int internalformat_,int width_,int height_,int depth_) {

    target 			= GL_TEXTURE_3D;
    setFormat(internalformat_);
    resize(width_,height_,depth_);
	setFilters(GL_NEAREST,GL_NEAREST);
	setWrapModes(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
}

void ncTexImage3D::setData(int index , void* data_) {

    glBindTexture(target,id);
    glTexSubImage3D(target,0,0,0,index,width,height,1,format,type,data_);
}

void ncTexImage3D::resize(int width_, int height_, int depth_) {

	if ((width!=width_) || (height!=height_) || (depth!=depth_))
	{
		width   = width_;
		height  = height_;
		depth   = depth_;

		glBindTexture(target,id);
		glTexImage3D(target,0,internalformat,width,height,depth,0,format,type,0);
	}
}




ncTexImageCube::ncTexImageCube(int internalformat_,int resolution) {

	depth			= 1;
	target 			= GL_TEXTURE_CUBE_MAP;
    setFormat(internalformat_);
    resize(resolution);
    setFilters(GL_NEAREST,GL_NEAREST);
	setWrapModes(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
}

void ncTexImageCube::setData(int index , void* data_) {

    glBindTexture(target,id);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+index,0,0,0,width,height,format,type,data_);
}

void ncTexImageCube::resize(int resolution) {

	if ((width!=resolution) || (height!=resolution))
	{

		width  = resolution;
		height = resolution;

		glBindTexture(target,id);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,internalformat,width,height,0,format,type,0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,internalformat,width,height,0,format,type,0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,internalformat,width,height,0,format,type,0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,internalformat,width,height,0,format,type,0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,internalformat,width,height,0,format,type,0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,internalformat,width,height,0,format,type,0);
	}

}




ncTexImageCubeArray::ncTexImageCubeArray(int internalformat_,int resolution, int depth_) {

	target 			= GL_TEXTURE_CUBE_MAP_ARRAY;
    setFormat(internalformat_);
    resize(resolution,depth_);
    setFilters(GL_NEAREST,GL_NEAREST);
	setWrapModes(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
}

void ncTexImageCubeArray::setData(int index , void* data_) {

    glBindTexture(target,id);
    glTexSubImage3D(target, 0, 0, 0, 6*index, width, height, 6, format, type, data_ );

}

void ncTexImageCubeArray::resize(int resolution, int depth_) {

    if ((width!=resolution) || (height!=resolution) || (depth!=depth_))
	{
		width  = resolution;
		height = resolution;
		depth  = depth_;

		glBindTexture(target,id);
		glTexImage3D(target, 0, internalformat, width, height, 6*depth, 0, format, type, 0);
	}

}
