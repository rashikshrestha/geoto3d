#version 400
#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_gpu_shader5 : enable

uniform mat4 MVP;

layout(triangles) in;
layout(triangle_Strip, max_vertices = 3) out;

out vec3    pNormal;

void main()
{
    pNormal = 0.5f+0.5f*normalize(cross(gl_in[1].gl_Position.xyz-gl_in[0].gl_Position.xyz , gl_in[2].gl_Position.xyz-gl_in[0].gl_Position.xyz));
    gl_Position = MVP*gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = MVP*gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = MVP*gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}
