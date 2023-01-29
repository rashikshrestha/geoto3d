#version 330 core
#extension GL_EXT_gpu_shader4 : enable

uniform samplerCube cube_tex;

in vec4 gPos;

layout(location = 0) out vec4 fColor;

void main(void)
{
	fColor = texture(cube_tex,normalize(gPos.xzy));
}
