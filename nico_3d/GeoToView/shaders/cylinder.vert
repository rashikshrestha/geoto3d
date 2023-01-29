#version 330 core
#extension GL_EXT_gpu_shader4 : enable

layout(location = 0) in vec4 mPosition;

void main(void)
{
	gl_Position = mPosition;                
}

