#version 330 core
#extension GL_EXT_gpu_shader4 : enable


uniform mat4	matMV;
uniform mat4	matP;
uniform	vec4	mColor;
uniform	float	mRadius;

layout(points) in;
layout(triangle_Strip, max_vertices = 4) out;

out vec4 gPos;
out vec4 gCenter;

void main()
{
	gl_PrimitiveID	= gl_PrimitiveIDIn;
	vec4 pos		= matMV * gl_in[0].gl_Position;
	gCenter			= pos;
	vec3 view		= -normalize(pos.xyz);
    vec3 center		= pos.xyz+mRadius*view;

    vec3 axes1= normalize(view.yzx-view.zxy);
    vec3 axes2= cross(view,axes1);

    axes1*=mRadius;
	axes2*=mRadius;

    gPos= vec4(center-axes1-axes2, 1.0f);
    gl_Position = matP * gPos;
    EmitVertex();

    gPos= vec4(center+axes1-axes2, 1.0f);
    gl_Position = matP * gPos;
    EmitVertex();

    gPos= vec4(center-axes1+axes2, 1.0f);
    gl_Position = matP * gPos;
    EmitVertex();

    gPos= vec4(center+axes1+axes2, 1.0f);
    gl_Position = matP * gPos;
    EmitVertex();

    EndPrimitive();
}
