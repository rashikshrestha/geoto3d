#version 330 core

layout(location = 0)    in		vec4 mPosition;

out     vec4        vColor;

uniform mat4        KI;
uniform int         width;
uniform int         height;
uniform sampler2D   cTex;

uniform sampler2DArray  iTex;
uniform int             ref;
uniform int             nrim;

void main() {

    int y = gl_VertexID/width;
    int x = gl_VertexID-y*width;

    vec2    rg  = texelFetch(cTex,ivec2(x,y),0).xy;
    float   d   = (rg.y>0.0f ? rg.x/rg.y : 0.0f);

    vec4 tmp = vec4((x+0.5f)/width,(y+0.5f)/height,d ,1.0f);
    tmp = KI*tmp;
    tmp/=tmp.w;

    vec3 itmp = vec3((x+0.5f)/width,(y+0.5f)/height,ref);
    vColor = textureLod(iTex,itmp,0);
    //vColor = mix(vec4(1.0f,0.0f,0.0f,0.0f),vec4(0.0f,1.0f,0.0f,0.0f),d);

    gl_Position = tmp;
}
