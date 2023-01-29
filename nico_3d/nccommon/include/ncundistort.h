#ifndef __NC_UNDISTORT__
#define __NC_UNDISTORT__

//#include <GL/glew.h>
#include "ncgl.h"
class ncProgram;
class ncFBO;
class ncCalibration;
class ncTexImage2D;
class ncTexImage2DArray;

class ncUndistort {

public:
	ncUndistort();
	~ncUndistort();

	void loadShaders();

	void undistort(ncTexImage2D* texIn, ncCalibration & cal_in, ncTexImage2D* texOut, ncCalibration & cal_out);
	void undistort(ncTexImage2D* texIn, ncTexImage2D* texOut, ncCalibration & cal);
	void undistort(ncTexImage2D* texIn, ncTexImage2DArray* texOut, int index, ncCalibration & cal);

public:
	ncProgram*  prog;
	ncFBO*      FBO;

	GLuint m_vao;
	GLuint m_vbo;

};

#endif
