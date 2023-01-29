#include "ncundistort.h"
#include "ncfbo.h"
#include "ncprogram.h"

#include "shaders/undistort.vert"
#include "shaders/undistort.frag"

ncUndistort::ncUndistort() {

	FBO     = new ncFBO;
	prog    = new ncProgram(UNDISTORT_VERTEX_SHADER , 0 , UNDISTORT_FRAGMENT_SHADER, true);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    float pdata[] = { 	-1.0,-1.0,1.0,1.0,
						 1.0,-1.0,1.0,1.0,
						-1.0, 1.0,1.0,1.0,
						 1.0, 1.0,1.0,1.0};
    glBufferData(GL_ARRAY_BUFFER, 16*sizeof(GLfloat), &pdata[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ncUndistort::~ncUndistort() {

	delete prog;

	glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void ncUndistort::undistort(ncTexImage2D* texIn, ncTexImage2D* texOut, ncCalibration & cal) {
    undistort(texIn, cal, texOut, cal);
}

void ncUndistort::undistort(ncTexImage2D* texIn, ncCalibration & cal_in, ncTexImage2D* texOut, ncCalibration & cal_out) {

	FBO->bind(GL_FRAMEBUFFER);
	FBO->attach(GL_COLOR_ATTACHMENT0,texOut,0,GL_FRAMEBUFFER);
	glViewport(0,0,texOut->width,texOut->height);

	glDisable(GL_SCISSOR_TEST);

	texIn->bind(0);
	texIn->setFilters(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR);
	texIn->setWrapModes(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
	texIn->generateMipmap();

	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	prog->Activate();
	prog->setSampler("tex", texIn, 0);
	prog->setUniform("matKGLi",cal_in.MV().inverse()*cal_out.MV()*cal_out.KGL(0).inverse());
	prog->setUniform("matKtex",cal_in.Ktex(0));
	prog->setUniform("kappa",cal_in.kappa());
	prog->setUniform("vignet",cal_in.getVignet());
	prog->setUniform("gamma",cal_in.getGamma());
	prog->setUniform("scale",cal_in.getScale());
	prog->setUniform("wb",cal_in.getWB());

	//fprintf(stderr,"vignet = %f\n",cal_in.getVignet());
	//fprintf(stderr,"gamma  = %f\n",cal_in.getGamma());
	//fprintf(stderr,"scale  = %f\n",cal_in.getScale());
	//fprintf(stderr,"wb     = %f %f %f\n",cal_in.getWB()[0],cal_in.getWB()[1],cal_in.getWB()[2]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	prog->Deactivate();
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	FBO->detach(GL_COLOR_ATTACHMENT0,GL_FRAMEBUFFER);
	FBO->unbind(GL_FRAMEBUFFER);

}

void ncUndistort::undistort(ncTexImage2D* texIn, ncTexImage2DArray* texOut, int index, ncCalibration & cal) {

	FBO->bind(GL_FRAMEBUFFER);
	FBO->attach(GL_COLOR_ATTACHMENT0,texOut,index,0,GL_FRAMEBUFFER);
	glViewport(0,0,texOut->width,texOut->height);

	texIn->bind(0);
	texIn->setFilters(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR);
	texIn->setWrapModes(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
	texIn->generateMipmap();

	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	prog->Activate();
	prog->setSampler ("tex", texIn, 0);
	prog->setUniform("matKGLi",cal.KGL(0).inverse());
	prog->setUniform("matKtex",cal.Ktex(0));
	prog->setUniform("kappa",cal.kappa());
	prog->setUniform("vignet",cal.getVignet());
	prog->setUniform("gamma",cal.getGamma());
	prog->setUniform("scale",cal.getScale());
	prog->setUniform("wb",cal.getWB());

	//fprintf(stderr,"vignet = %f\n",cal.getVignet());
	//fprintf(stderr,"gamma  = %f\n",cal.getGamma());
	//fprintf(stderr,"scale  = %f\n",cal.getScale());
	//fprintf(stderr,"wb     = %f %f %f\n",cal.getWB()[0],cal.getWB()[1],cal.getWB()[2]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	FBO->detach(GL_COLOR_ATTACHMENT0,GL_FRAMEBUFFER);
	FBO->unbind(GL_FRAMEBUFFER);

}
