#version 330 core
#extension GL_NV_shader_buffer_load : enable

uniform sampler2D   cTex;
uniform float*      gFact;
uniform int         gSize;
uniform int         lod;
out     vec2        fColor;

void main() {

	ivec2 tc     = ivec2(gl_FragCoord.xy);
    ivec2 sz     = textureSize(cTex,lod);
    vec2  result = gFact[0]*texelFetch(cTex,tc,lod).xy;
    for (int i=1;i<gSize;++i)
        result += gFact[i]* ( texelFetch(cTex,ivec2(max(0,tc.x-i),tc.y),lod).xy + texelFetch(cTex,ivec2(min(tc.x+i,sz.x-1),tc.y),lod).xy );
    fColor = result;
}
