#ifndef NCRENDERBUFFER_H
#define NCRENDERBUFFER_H

#include "ncgl.h"

class ncRenderBuffer {

public:

	 ncRenderBuffer(int iformat,int w,int h);
	~ncRenderBuffer();

	void bind() const;
    void resize(int w,int h);
	void setFormat(GLenum iformat);

	GLuint					id;
	GLenum 					target;
	int 					internalformat;
	int						width;
	int						height;
	GLenum 					format;
	GLenum					type;
	unsigned int            bpp;

};

#endif // NCRENDERBUFFER_H
