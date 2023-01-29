#ifndef NCTEXIMAGE_H
#define NCTEXIMAGE_H

#include "ncgl.h"
#include "ncmath.h"
class ncProgram;

class ncTexImage {

public:

	 ncTexImage();
	~ncTexImage();

	void bind() const;
	void bind(GLenum texunit) const;

	void setFilters(GLenum min, GLenum mag);
	void setWrapModes(GLenum wx, GLenum wy, GLenum wz = GL_CLAMP_TO_EDGE);
	void generateMipmap();

	void setFormat(GLenum iformat);

	GLuint					id;
	GLenum 					target;
	int 					internalformat;
	int						width;
	int						height;
	int						depth;
	GLenum 					format;
	GLenum					type;
	unsigned int            bpp;

};


class ncTexImageRect : public ncTexImage
{
	public:
		ncTexImageRect(int internalformat_,int width_,int height_);

        void setData(void* data_);
        void resize(int width_, int height_);
};

class ncTexImage2D : public ncTexImage
{
	public:
		ncTexImage2D(int internalformat_,int width_,int height_);

        void setData(void* data_);
        void resize(int width_, int height_);
        void setAnisotropy(float aniso);

        void draw(ncc::m4x4f color_matrix , ncc::v4f color_offset = ncc::v4f(0.0f,0.0f,0.0f,0.0f) , bool flip = false);

    private:
        static ncProgram* draw_prog;

};

class ncTexImageMultisample2D : public ncTexImage
{
	public:
		ncTexImageMultisample2D(int internalformat_,int width_,int height_, int nrsamples);
        void resize(int width_, int height_);

    int nrsamples;
};

class ncTexImage2DArray : public ncTexImage
{
	public:
		ncTexImage2DArray(int internalformat_,int width_,int height_,int depth_);

        void setData(int index , void* data_);
        void resize(int width_, int height_, int depth_);
};

class ncTexImage3D : public ncTexImage
{
	public:
		ncTexImage3D(int internalformat_,int width_,int height_,int depth_);

        void setData(int index , void* data_);
        void resize(int width_, int height_, int depth_);
};

class ncTexImageCube : public ncTexImage
{
	public:
		ncTexImageCube(int internalformat_,int resolution_);

        void setData(int index , void* data_);
        void resize(int resolution);

};

class ncTexImageCubeArray : public ncTexImage
{
	public:
		ncTexImageCubeArray(int internalformat_,int resolution, int depth_);

        void setData(int index , void* data_);
        void resize(int resolution, int depth_);

};

#endif // NCTEXIMAGE_H
