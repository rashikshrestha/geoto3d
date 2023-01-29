#ifndef NCPBO_H
#define NCPBO_H

//#include <GL/glew.h>
#include "ncgl.h"
#include <stdio.h>

class ncPBO
{
	public:

		ncPBO(GLenum type = GL_STATIC_DRAW , GLenum mode = GL_READ_WRITE);

		template <class T>
		ncPBO(int nrelements_ = 0, T * data = 0 , GLenum type = GL_STATIC_DRAW , GLenum mode = GL_READ_WRITE) {
            init();
            setup(nrelements_, data , type , mode);
        }

		~ncPBO();

		template <class T>
		void setup(int nrelements_, T * data = 0 , GLenum type = GL_STATIC_DRAW , GLenum mode = GL_READ_WRITE) {
            nrelements = nrelements_;
            setup_void(nrelements_* sizeof(T),(void*)data , type , mode);
        }

		void   	    bind(GLenum target);
		void 	    unbind(GLenum target);
		GLuint 	    getID();
        GLuint64EXT getGpuAddress();

		void makeResident(GLenum mode = GL_READ_WRITE);
		void makeNonResident();
		bool isResident();


	GLuint 			        id;
	int 			        nrelements;
	unsigned int 	        size;

private:
	void            setup_void(int size_ , void* data = 0 , GLenum type = GL_STATIC_DRAW , GLenum mode = GL_READ_WRITE);
	void            init();
	GLuint64EXT     gpu_addr;
	GLenum          gpu_mode;


};

#endif // NCPBO_H
