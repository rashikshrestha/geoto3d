out     vec2 fColor;

void main() {

    ivec2 tc    = ivec2(gl_FragCoord.xy);
    ivec3 sz    = textureSize(zTex,0);
    float berror= 0.0f;
    float bdepth= 0.0f;

#if 1
    ivec2 otc[9];
    otc[ 0]= ivec2(-1,-1);
    otc[ 1]= ivec2(-1, 0);
    otc[ 2]= ivec2(-1, 1);
    otc[ 3]= ivec2( 0,-1);
    otc[ 4]= ivec2( 0, 0);
    otc[ 5]= ivec2( 0,+1);
    otc[ 6]= ivec2( 1,-1);
    otc[ 7]= ivec2( 1, 0);
    otc[ 8]= ivec2( 1,+1);

    float pref[9];
    for (int i=0;i<9;++i)
        pref[i] = texelFetch(zTex,ivec3(tc.xy+otc[i],ref),0).x;

    for (int i=0;i<9;++i)
    {
        float tmp = 0.0f;
        for (int j=0;j<9;++j)
            tmp += exp(-(pref[j]-pref[i])*(pref[j]-pref[i])/0.01f);
        tmp/=9.0f;

        for (int k=-1;k<=1;++k)
        {
            float zreal = Zn/(1.0f-pref[i]);
            float d  = 1.0f - Zn/(zreal+k*0.001f);
            //float d = pref[i];

            float error = 0.75f*getErrorPass1(d)+0.25f*tmp;

            if (error>=berror)
            {
                berror = error;
                bdepth = d;
            }
        }
    }

#else
    float pref[9];
    for (int i=-1;i<=1;++i)
        for (int j=-1;j<=1;++j)
            pref[3*(j+1)+(i+1)] = texelFetch(zTex,ivec3(tc.x+i,tc.y+j,ref),0).x;

    for (int i=-1;i<=1;++i)
        for (int j=-1;j<=1;++j)
        {
            int index = 3*(j+1)+(i+1);

            for (int k=-4;k<=4;++k)
            {
                float fd    = clamp((1.0f+k*0.1f)*pref[index],0.0f,1.0f);
                float d     = calcDepth(fd);

                float tmp = 0.0f;
                for (int l=0;l<9;++l)
                    tmp += exp(-(pref[l]-d)*(pref[l]-d)/0.001f);
                tmp/=9.0f;
                float error = 0.5f*getErrorPass1(d)+0.5f*tmp;

                if (error>=berror)
                {
                    berror = error;
                    bdepth = d;
                }
            }
        }
#endif
    fColor          = vec2(bdepth,1.0f);

    gl_FragDepth    = bdepth;
}

