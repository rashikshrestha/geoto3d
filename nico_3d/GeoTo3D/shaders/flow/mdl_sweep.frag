layout(location = 0) out     vec4  fColor;
layout(location = 1) out     vec2  fDepth;

void main() {

    vec4 tc_ref = Ptex[ref]*mPos;
    tc_ref/=tc_ref.w;

    vec3 fp     = vec3( tc_ref.xy ,ref);
    vec2 dref 	= textureLod(zTex, fp, 0).xy;
    vec4 cref   = textureLod(cTex, fp, 0);

    if ((cref.w==1.0f) && (dref.y == 1.0f))
    {
        fColor = cref;
        fDepth = dref;
    }
    else
        discard;
}

