#version 420 core
#extension GL_NV_shader_buffer_load : enable

uniform sampler2DArray  zTex;

uniform mat4*           Ptex;
uniform int             nrim;
uniform int             ref;

in vec4  p2D;
in vec2  dval;

out vec2    fColor;

void main() {

    if (dval.y==1.0f)
    {
        for (int i=0;i<nrim;++i)
        {
            vec4 tc = Ptex[i]*p2D;
            if (tc.w>0.0f)
            {
                tc/=tc.w;
                if ((tc.x>0.0f) && (tc.x<1.0f) && (tc.y>0.0f) && (tc.y<1.0f))
                {
                    vec2 dval = texture(zTex,vec3(tc.x,tc.y,i)).xy;
                    if ((dval.y==1.0f) && (dval.x > tc.z))
                        discard;

                }
            }
        }

        fColor          = vec2(gl_FragCoord.z , 1.0f);
    }
    else
        discard;
}
