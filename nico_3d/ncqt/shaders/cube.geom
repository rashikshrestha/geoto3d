#version 330 core
#extension GL_EXT_gpu_shader4 : enable

uniform mat4	matP;
uniform	float	mRadius;

layout(points) in;
layout(triangle_Strip, max_vertices = 14) out;

out vec4 gPos;

void main()
{
	vec4 p0 = gl_in[0].gl_Position + vec4(-mRadius,-mRadius,-mRadius,0.0f);
	vec4 p1 = gl_in[0].gl_Position + vec4(+mRadius,-mRadius,-mRadius,0.0f);
	vec4 p2 = gl_in[0].gl_Position + vec4(-mRadius,+mRadius,-mRadius,0.0f);
	vec4 p3 = gl_in[0].gl_Position + vec4(+mRadius,+mRadius,-mRadius,0.0f);
	vec4 p4 = gl_in[0].gl_Position + vec4(-mRadius,-mRadius,+mRadius,0.0f);
	vec4 p5 = gl_in[0].gl_Position + vec4(+mRadius,-mRadius,+mRadius,0.0f);
	vec4 p6 = gl_in[0].gl_Position + vec4(-mRadius,+mRadius,+mRadius,0.0f);
	vec4 p7 = gl_in[0].gl_Position + vec4(+mRadius,+mRadius,+mRadius,0.0f);

	gPos= p3;
    gl_Position = matP*p3;
    EmitVertex();
    gPos= p2;
    gl_Position = matP*p2;
    EmitVertex();
    gPos= p7;
    gl_Position = matP*p7;
    EmitVertex();
    gPos= p6;
    gl_Position = matP*p6;
    EmitVertex();
    gPos= p4;
    gl_Position = matP*p4;
    EmitVertex();
    gPos= p2;
    gl_Position = matP*p2;
    EmitVertex();
    gPos= p0;
    gl_Position = matP*p0;
    EmitVertex();
    gPos= p3;
    gl_Position = matP*p3;
    EmitVertex();
    gPos= p1;
    gl_Position = matP*p1;
    EmitVertex();
    gPos= p7;
    gl_Position = matP*p7;
    EmitVertex();
    gPos= p5;
    gl_Position = matP*p5;
    EmitVertex();
    gPos= p4;
    gl_Position = matP*p4;
    EmitVertex();
    gPos= p1;
    gl_Position = matP*p1;
    EmitVertex();
    gPos= p0;
    gl_Position = matP*p0;
    EmitVertex();

    EndPrimitive();
}
