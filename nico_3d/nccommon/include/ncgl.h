#ifndef __NC_GL_H__
#define __NC_GL_H__

//!GLSL "#" directives can't be inside MAKE_GLSL_STRING, put before MAKE_GLSL_STRING!
#define MAKE_GLSL_STRING(A) #A
#define BUFFER_OFFSET(i) ((char*)NULL + (i))

#ifdef WIN32
#include <windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#else
#include <GL/glew.h>
//#include <GL/glxew.h>
//#define GL_GLEXT_PROTOTYPES 1
//#include <GL/gl.h>
//#include <GL/glext.h>
//#include <GL/glu.h>
#endif

#include "nccommon.h"

class ncFBO;
class ncUndistort;

class ncgl {

public:
    static GLenum init(std::string* msg = 0);

	static int
	getGPUTemp();

	static void
	checkErrors();

	static void
	checkFBO();

    static ncFBO*           FBO1;
    static ncFBO*           FBO2;
    static ncUndistort*     undistorter;
};

#endif //__NC_GL_H__
