#version 400
#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_gpu_shader5 : enable

layout(location = 0) in ivec4 mPosition;

out float   edge;

void main(void)
{
    vec3 p3D            = 0.001f*(mPosition.xyz);
    gl_Position         = vec4( p3D ,1.0f);
    edge                = 0.001f*(1<<mPosition.w);
}
