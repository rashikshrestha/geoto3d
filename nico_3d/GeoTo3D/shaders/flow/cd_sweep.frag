out     vec2  fColor;

vec2 myTest() {

    ivec3 fp    = ivec3(gl_FragCoord.xy,ref);
    vec2  depth = texelFetch(zTex, fp, 0).xy;
    vec2 result = vec2(depth.x,0.0f);

    int valid_count = 0;

    for (int i=0;i<nrim;++i)
    {
        if (i!=ref)
        {
            vec4 tc = Ptex[i]*vec4(mPos.xy,mPos.z+depth.x,mPos.w);
            if (tc.w>0.0f)
            {
                tc/=tc.w;

                if ((tc.x>0.0f) && (tc.x<1.0f) && (tc.y>0.0f) && (tc.y<1.0))
                {
                    vec2 sens = textureLod(zTex,vec3(tc.x,tc.y,i),0).xy;

				    float refZ	= Zn/(1.0f-tc.z		);
				    float senZ	= Zn/(1.0f-sens.x	);
				    if (abs(senZ-refZ)<0.05)
				    	valid_count++;
                }
            }
        }
    }

    if (valid_count>=3)
        result.y = depth.y;

    return result;
}


void main() {

    fColor = myTest();
}
