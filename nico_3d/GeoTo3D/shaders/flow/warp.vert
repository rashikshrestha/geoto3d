#version 330 core

uniform mat4        MVP;
uniform sampler2D   cTex;
uniform sampler2D   dTex;

out float zpos;
out vec4  color;

void main() {

    ivec2   sz  = textureSize(dTex,0);

    int     y   = gl_VertexID/sz.x;
    int     x   = gl_VertexID-y*sz.x;
    float   d   = texelFetch(dTex,ivec2(x,y),0).x;
    color       = texelFetch(cTex,ivec2(x,y),0);

    vec4 tmp    = vec4((x+0.5f)/sz.x,(y+0.5f)/sz.y,d ,1.0f);
    tmp         = MVP*tmp;
    zpos        = 0.5f+0.5f*tmp.z/tmp.w;
    gl_Position = tmp;
}
