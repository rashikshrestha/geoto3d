#version 330 core

uniform	sampler2DRect tex;

layout(location = 0) out vec4 fColor;

void main(void)
{
	ivec2 tc	= ivec2(gl_FragCoord.xy);
	fColor		= texelFetch(tex,tc);
}
