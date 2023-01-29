#version 400
#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_gpu_shader5 : enable

layout(points) in;
layout(triangle_Strip, max_vertices = 14) out;

in  float   edge[1];

out vec3    p3D;

uniform mat4 MVP;

void main()
{
    vec4 p0,p1,p2,p3,p4,p5,p6,p7;

    p0 = p1 = p2 = p3 = p4 = p5 = p6 = p7 = gl_in[0].gl_Position;

    p1.x += edge[0];

    p2.y += edge[0];

    p3.x += edge[0];
    p3.y += edge[0];

    p4.z += edge[0];

    p5.x += edge[0];
    p5.z += edge[0];

    p6.y += edge[0];
    p6.z += edge[0];

    p7.x += edge[0];
    p7.y += edge[0];
    p7.z += edge[0];

    gl_Position = MVP*p3;
    p3D = p3.xyz;
    EmitVertex();
    gl_Position = MVP*p2;
    p3D = p2.xyz;
    EmitVertex();
    gl_Position = MVP*p7;
    p3D = p7.xyz;
    EmitVertex();
    gl_Position = MVP*p6;
    p3D = p6.xyz;
    EmitVertex();
    gl_Position = MVP*p4;
    p3D = p4.xyz;
    EmitVertex();
    gl_Position = MVP*p2;
    p3D = p2.xyz;
    EmitVertex();
    gl_Position = MVP*p0;
    p3D = p0.xyz;
    EmitVertex();
    gl_Position = MVP*p3;
    p3D = p3.xyz;
    EmitVertex();
    gl_Position = MVP*p1;
    p3D = p1.xyz;
    EmitVertex();
    gl_Position = MVP*p7;
    p3D = p7.xyz;
    EmitVertex();
    gl_Position = MVP*p5;
    p3D = p5.xyz;
    EmitVertex();
    gl_Position = MVP*p4;
    p3D = p4.xyz;
    EmitVertex();
    gl_Position = MVP*p1;
    p3D = p1.xyz;
    EmitVertex();
    gl_Position = MVP*p0;
    p3D = p0.xyz;
    EmitVertex();

    EndPrimitive();
}
