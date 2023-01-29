out     vec4            fColor;

void main() {

    float bdepth = 0.0f;
    float berror = 0.0f;

    for (float fd = 0.0f;fd<1.0f;fd+=0.001f)
    {
        float d     = calcDepth(fd);
        float error = getErrorPass1(d);

        if (error>=berror)
        {
            berror = error;
            bdepth = d;
        }
    }

    fColor = vec4(bdepth,1.0f/*berror*/,0.0f,0.0f);
    gl_FragDepth = bdepth;

}

