#version 400 core
#extension GL_NV_shader_buffer_load : enable

in  vec3    pNormal;
out vec4    fColor;

void main(void)
{
    fColor = vec4(pNormal,0.0f);
}
