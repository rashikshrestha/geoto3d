#version 330 core
#extension GL_EXT_gpu_shader4 : enable

uniform	mat4	matMV;
uniform mat4	matP;
uniform	float	mRadius;

layout(lines) in;
layout(triangle_Strip, max_vertices = 14) out;

out vec4 gPos;
out vec4 gBase;
out vec4 gTop;

void main()
{
	gl_PrimitiveID = gl_PrimitiveIDIn;

	vec4 base= gl_in[0].gl_Position;
	vec4 top= gl_in[1].gl_Position;

	gTop= matMV*top;
	gBase= matMV*base;

	vec4 axes = vec4(top.xyz - base.xyz , 0.0f);
	vec4 axes1= vec4(mRadius*normalize(      axes.yzx-axes.zxy  ),0.0f);
	vec4 axes2= vec4(mRadius*normalize(cross(axes.xyz,axes1.xyz)),0.0f);

	vec4 p0 = matMV*(base-axes1-axes2);
	vec4 p1 = matMV*(base+axes1-axes2);
	vec4 p2 = matMV*(base-axes1+axes2);
	vec4 p3 = matMV*(base+axes1+axes2);
	vec4 p4 = matMV*(top -axes1-axes2);
	vec4 p5 = matMV*(top +axes1-axes2);
	vec4 p6 = matMV*(top -axes1+axes2);
	vec4 p7 = matMV*(top +axes1+axes2);

	gPos=p3;
	gl_Position=matP*p3;
	EmitVertex();
	gPos=p2;
	gl_Position=matP*p2;
	EmitVertex();
	gPos=p7;
	gl_Position=matP*p7;
	EmitVertex();
	gPos=p6;
	gl_Position=matP*p6;
	EmitVertex();
	gPos=p4;
	gl_Position=matP*p4;
	EmitVertex();
	gPos=p2;
	gl_Position=matP*p2;
	EmitVertex();
	gPos=p0;
	gl_Position=matP*p0;
	EmitVertex();
	gPos=p3;
	gl_Position=matP*p3;
	EmitVertex();
	gPos=p1;
	gl_Position=matP*p1;
	EmitVertex();
	gPos=p7;
	gl_Position=matP*p7;
	EmitVertex();
	gPos=p5;
	gl_Position=matP*p5;
	EmitVertex();
	gPos=p4;
	gl_Position=matP*p4;
	EmitVertex();
	gPos=p1;
	gl_Position=matP*p1;
	EmitVertex();
	gPos=p0;
	gl_Position=matP*p0;
	EmitVertex();
	
	EndPrimitive();
}
