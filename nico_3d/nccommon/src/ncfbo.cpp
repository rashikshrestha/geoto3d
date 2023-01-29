#include "ncfbo.h"
#include "ncrenderbuffer.h"
#include "ncteximage.h"

ncFBO::ncFBO() {
	glGenFramebuffers(1, &id);
}

ncFBO::~ncFBO() {
	glDeleteFramebuffers(1,&id);
}

void ncFBO::bind(GLenum target) {

	glBindFramebuffer(target,id);
}

void ncFBO::unbind(GLenum target) {

	glBindFramebuffer(target,0);
}

void ncFBO::attach(GLenum attachment, ncTexImageRect* tex, GLenum target) {

	glBindFramebuffer(target,id);
	glFramebufferTexture2D(target,attachment,tex->target,tex->id,0);
}

void ncFBO::attach(GLenum attachment, ncTexImage2D* tex, int level, GLenum target) {

	glBindFramebuffer(target,id);
	glFramebufferTexture2D(target,attachment,tex->target,tex->id,level);
}

void ncFBO::attach(GLenum attachment, ncTexImageMultisample2D* tex, int level, GLenum target) {

	glBindFramebuffer(target,id);
	glFramebufferTexture2D(target,attachment,tex->target,tex->id,level);
}

void ncFBO::attach(GLenum attachment, ncTexImage2DArray* tex, int layer, int level, GLenum target) {

	glBindFramebuffer(target,id);
	glFramebufferTextureLayer(target,attachment,tex->id,level,layer);
}

void ncFBO::attach(GLenum attachment, ncTexImage3D* tex, int layer, int level, GLenum target) {

	glBindFramebuffer(target,id);
	glFramebufferTexture3D( target,attachment,tex->target,tex->id,level,layer);

}

void ncFBO::attach(GLenum attachment, ncTexImageCube* tex, int level, GLenum target) {

	glBindFramebuffer(target,id);
	glFramebufferTexture(target,attachment,tex->id,level);
}

void ncFBO::attach(GLenum attachment, ncRenderBuffer* rbuf, GLenum target) {
	glBindFramebuffer(target,id);
	glFramebufferRenderbuffer(target,attachment,rbuf->target,rbuf->id);
}

void ncFBO::detach(GLenum attachment, GLenum target) {

	glBindFramebuffer(target,id);

	int object_type;
	glGetFramebufferAttachmentParameteriv(target,attachment,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,&object_type);

	switch(object_type) {
		case GL_TEXTURE:		glFramebufferTextureLayer(target,attachment,0,0,0);
								break;
		case GL_RENDERBUFFER:	glFramebufferRenderbuffer(target,attachment,0,0);
								break;
	}
}

