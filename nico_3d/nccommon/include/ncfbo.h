#ifndef NCFBO_H
#define NCFBO_H

#include "ncgl.h"

class ncTexImageRect;
class ncTexImage2D;
class ncTexImageMultisample2D;
class ncTexImage2DArray;
class ncTexImage3D;
class ncTexImageCube;

class ncRenderBuffer;

class ncFBO
{
	public:
		ncFBO();
		~ncFBO();

		void bind(GLenum target = GL_FRAMEBUFFER);
		void unbind(GLenum target = GL_FRAMEBUFFER);

		void attach(GLenum attachment, ncTexImageRect* tex, GLenum target = GL_FRAMEBUFFER);
		void attach(GLenum attachment, ncTexImage2D* tex, int level = 0, GLenum target = GL_FRAMEBUFFER);
		void attach(GLenum attachment, ncTexImageMultisample2D* tex, int level = 0, GLenum target = GL_FRAMEBUFFER);
		void attach(GLenum attachment, ncTexImage2DArray* tex, int layer, int level = 0, GLenum target = GL_FRAMEBUFFER);
		void attach(GLenum attachment, ncTexImage3D* tex, int layer, int level = 0, GLenum target = GL_FRAMEBUFFER);
		void attach(GLenum attachment, ncTexImageCube* tex, int level = 0, GLenum target = GL_FRAMEBUFFER);

		void attach(GLenum attachment, ncRenderBuffer* rbuf, GLenum target = GL_FRAMEBUFFER);

		void detach(GLenum attachment, GLenum target = GL_FRAMEBUFFER);

		GLuint id;
};

#endif // NCFBO_H
