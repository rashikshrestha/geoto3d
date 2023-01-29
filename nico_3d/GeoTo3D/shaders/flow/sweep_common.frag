#version 420 core
#extension GL_NV_shader_buffer_load : enable

uniform sampler2DArray  cTex;
uniform sampler2DArray  zTex;
uniform mat4*           Ptex;
uniform float*          Zlut;
uniform int             nrim;
uniform int             ref;
uniform float           Zn;
in      vec4            mPos;

vec4 getColor(vec2 tc2, int index) {

    return texture(cTex,vec3(tc2.x,tc2.y,index));
}

vec2 getDepth(vec2 tc2, int index) {

    return texture(zTex,vec3(tc2.x,tc2.y,index)).xy;
}

float getErrorPass1(float depth) {

	float 	range = 0.5f*nrim;
    vec4    csen;
    vec4    cref;

    cref = getColor(mPos.xy,ref);

    float 	total_value  = 1.0f;
    float 	total_weight = 1.0f;
    //vec3  total_blend  = cref.xyz;
    float 	value,weight;

	float 	w_col = 0.0f;
	float 	w_der = 0.0f;
	float 	w_lap = 1.0f;

	float 	w_norm 		= 1.0f/(w_col + w_der + w_lap);
	vec3 	chan_weight = w_norm*vec3(w_col,w_der,w_lap);

	float sigma = 0.05;

    for (int i=0;i<nrim;++i)
    {
        if (i!=ref)
        {
            vec4 tc = Ptex[i]*vec4(mPos.xy,mPos.z+depth,mPos.w);
            if (tc.w>0.0f)
            {
                tc/=tc.w;
                csen            = getColor(tc.xy,i);
                vec3 tmp        = abs(csen.xyz-cref.xyz);

                tmp 			= tmp*chan_weight;
                value           = exp(-0.5*dot(tmp,tmp)/(sigma*sigma));
                weight          = csen.w;

                total_weight   += weight;
                total_value    += weight*value;
				//total_blend	   += weight*csen.xyz;
            }
        }
    }

    //float count_weight = 0.05f;
	//return (1.0f-count_weight)*(total_value/total_weight) + count_weight*(total_weight/nrim);

	if ((cref.w==1.0f) && (total_weight>=range))
		return total_value/total_weight;
	else
		return 0.01f;
}
