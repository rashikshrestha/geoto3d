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
        result += gFact[i]* ( texelFetch(cTex,ivec2(tc.x,max(tc.y-i,0)),lod) + texelFetch(cTex,ivec2(tc.x,min(tc.y+i,sz.y-1)),lod) );

    //fColor = vec4(result.xyz,value.w);
    fColor = result;
}
