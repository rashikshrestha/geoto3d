#version 330

uniform mat4 KGL;
uniform mat4 MVi;

layout(triangles) in;
layout(triangle_Strip, max_vertices = 3) out;

in vec2     tc_in[3];
in vec4     color_in[3];

out vec3    pNormal;
out vec2    tc;
out vec4    color;

void main()
{
    mat4 MVP = KGL*MVi;

    pNormal 	= 0.5f+0.5f*normalize(cross(gl_in[1].gl_Position.xyz-gl_in[0].gl_Position.xyz , gl_in[2].gl_Position.xyz-gl_in[0].gl_Position.xyz));
    tc          = tc_in[0];
    color       = color_in[0];
    gl_Position = MVP*gl_in[0].gl_Position;
    EmitVertex();
    tc          = tc_in[1];
    color       = color_in[1];
    gl_Position = MVP*gl_in[1].gl_Position;
    EmitVertex();
    tc          = tc_in[2];
    color       = color_in[2];
    gl_Position = MVP*gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
