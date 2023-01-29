#version 400 core
#extension GL_NV_shader_buffer_load : enable

uniform sampler2D   cTex;
uniform int         lod;

out vec4 fColor;

void main() {

    ivec2 tc = ivec2(gl_FragCoord.xy);

    float edge_x = texelFetch(cTex,ivec2(tc.x+1,tc.y),lod).x - texelFetch(cTex,ivec2(tc.x-1,tc.y),lod).x;
    float edge_y = texelFetch(cTex,ivec2(tc.x,tc.y+1),lod).x - texelFetch(cTex,ivec2(tc.x,tc.y-1),lod).x;

    float result = sqrt(edge_x*edge_x+edge_y*edge_y);

    fColor = vec4(10.0f*result);
}
