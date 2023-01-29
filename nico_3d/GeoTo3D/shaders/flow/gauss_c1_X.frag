#version 330 core
#extension GL_NV_shader_buffer_load : enable

uniform sampler2D   cTex;
uniform float*      gFact;
uniform int         gSize;
uniform int         lod;
out     vec4        fColor;

void main() {

	ivec2 tc = ivec2(gl_FragCoord.xy);
    ivec2 sz = textureSize(cTex,lod);

    vec4 value  = texelFetch(cTex,tc,lod);
    vec4 result = gFact[0]*value;

    for (int i=1;i<gSize;++i)
        result += gFact[i]* ( texelFetch(cTex,ivec2(max(0,tc.x-i),tc.y),lod) + texelFetch(cTex,ivec2(min(tc.x+i,sz.x-1),tc.y),lod) );

    //fColor = vec4(result.xyz,value.w);
    fColor = result;
}
