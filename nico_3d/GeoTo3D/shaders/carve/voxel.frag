out         vec4    fColor;

flat in     int     gIndex;

uniform mat4    Ktex;
uniform mat4    Ktexi;
uniform float   roi;
uniform vec2    tc_scale;

void main(void)
{
    vec2  tc    = tc_scale*gl_FragCoord.xy;
    vec2 ztex   = textureLod(dTex,tc,0).xy;

    if (ztex.y != 1.0f)
        discard;
    else
    {
        vec4  ray1  = Ktexi*vec4(tc,0.0,1.0f);
        ray1/=ray1.w;
        vec4  tmp   = Ktex*vec4( roi*normalize(ray1.xyz) ,1.0f);
        float zmax  = tmp.z/tmp.w;


        if (gl_FragCoord.z < min(zmax,ztex.x) )
            atomicOr(vPBO+gIndex, 1);
        else
            atomicOr(vPBO+gIndex, 2);
    }
}
