out     vec4            fColor;

void main() {

    float bdepth = 0.0f;
    float berror = 0.0f;

    ivec3 fp    = ivec3(gl_FragCoord.xy,ref);
    vec2  tmp   = texelFetch(zTex, fp, 0).xy;

    if (tmp.y==0.0f)
    {

        for (float fd = 0.0f;fd<1.0f;fd+=0.001f)
        {
            float d     = calcDepth(fd);
            float error = getErrorPass2(d);

            if (error>=berror)
            {
                berror = error;
                bdepth = d;
            }
        }

        fColor = vec4(bdepth,1.0f/*berror*/,0.0f,0.0f);
        gl_FragDepth = bdepth;
    }
    else
    {
        fColor = vec4(tmp.x,1.0f/*berror*/,0.0f,0.0f);
        gl_FragDepth = tmp.x;

    }

}

