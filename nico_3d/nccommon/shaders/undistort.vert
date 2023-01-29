const char UNDISTORT_VERTEX_SHADER[] = "						                \n\
                                                                                \n\
#version 110                                                                    \n\
                                                                                \n\
uniform mat4 	matKGLi;                                                        \n\
                                                                                \n\
varying	vec4 	vPos;                                                           \n\
                                                                                \n\
void main(void)                                                                 \n\
{                                                                               \n\
	gl_Position 	= gl_Vertex;                                                \n\
	vPos			= matKGLi*gl_Vertex;                                        \n\
}                                                                               ";

