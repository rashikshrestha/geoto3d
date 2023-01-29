#include "ncgl.h"
#include "ncfbo.h"
#include "ncundistort.h"

ncFBO*          ncgl::FBO1          = 0;
ncFBO*          ncgl::FBO2          = 0;
ncUndistort*    ncgl::undistorter   = 0;

GLenum
ncgl::init(std::string* msg) {

	GLenum err = glewInit();
	if (err == GLEW_OK)
	{
		const GLubyte* glversion = glGetString(GL_VERSION);
		fprintf(stderr,"OpenGL Version: %s\n",glversion);

        glDisable(GL_DITHER);

		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glPixelStorei(GL_PACK_ALIGNMENT,1);

		if (GLEW_VERSION_2_0)
		{
			FBO1        = new ncFBO;
			FBO2        = new ncFBO;
			undistorter = new ncUndistort;
		}
	}
	else
	{
		if (msg)
		{
			char tmp[1024];
			sprintf(tmp,"Error: %s\n", glewGetErrorString(err));
			*msg = tmp;
		}
	}
	return err;
};

int
ncgl::getGPUTemp() {

#ifdef WIN32
	return 0;
#else
	int gputemp;
	FILE *cmd=popen(" nvidia-settings -t -q GPUCoreTemp","r");
	fscanf (cmd, "%d", &gputemp);
	pclose(cmd);
	return gputemp;
#endif
}

void
ncgl::checkErrors() {
    GLenum error;
    do {
		error = glGetError();
        fprintf(stderr, "GL Error (%X): %s", error,(char *) gluErrorString(error));
		if (error != GL_NO_ERROR)
			fprintf(stderr," !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		else
			fprintf(stderr,"\n");

	} while (error != GL_NO_ERROR);
}

void
ncgl::checkFBO() {
    GLenum error;

	error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (error != GL_FRAMEBUFFER_COMPLETE)
        fprintf(stderr, "FBO Error (%X) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", error);
	else
		fprintf(stderr,"\n");
}

