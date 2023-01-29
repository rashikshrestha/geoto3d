#version 420 core

uniform sampler2DArray  zTex;

uniform mat4            MVP;
uniform int             ref;

out vec4  p2D;
out vec2  dval;

void main() {

    ivec3   sz  = textureSize(zTex,0);
    int     y   = gl_VertexID/sz.x;
    int     x   = gl_VertexID-y*sz.x;

    dval        = texelFetch(zTex,ivec3(x,y,ref),0).xy;
    p2D         = vec4((x+0.5f)/sz.x,(y+0.5f)/sz.y,dval.x ,1.0f);
    gl_Position = MVP*p2D;
}
