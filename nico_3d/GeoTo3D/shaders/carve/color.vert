#version 400 core
#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_gpu_shader5 : enable

layout(location = 0)    in		vec3 mPosition;

uniform sampler2D   cTex;
uniform sampler2D   dTex;

uniform vec4        *mColor;
uniform mat4        MVPtex;

uniform vec3 campos;

void main(void)
{
	vec4 tmp        = MVPtex*vec4(mPosition,1.0f);

	gl_Position 	= tmp;

    float border = 0.05f;

    if (tmp.w>0.0f)
    {
        tmp/=tmp.w;

        if ((tmp.x>border) && (tmp.x<1.0f-border) && (tmp.y>border) && (tmp.y<1.0f-border) && (tmp.z>border) && (tmp.z<1.0f-border))
        {
            float ref       = texture(dTex,tmp.xy).x;
            float weight    = pow(1.0f-ref,5.0f);
            vec3  color     = texture(cTex,tmp.xy).xyz;

            if (tmp.z-0.005f<=ref)
                mColor[gl_VertexID] += weight*vec4(color,1.0f);
         }
    }
}
