#version 330 core

uniform sampler2D   cTex;
uniform int         lod;

out     vec4        fColor;

void main() {

	ivec2 tc = ivec2(gl_FragCoord.xy);
    ivec2 sz = textureSize(cTex,lod);

	vec4 	result 	= texelFetch(cTex,tc,lod);
	int 	border 	= max(8,min(sz.x,sz.y)/100);
    if ( (tc.x<border) || (tc.x>sz.x-1-border) || (tc.y<border) || (tc.y>sz.y-1-border) )
    	result.w = 0.0f;
	fColor	= result;

}
