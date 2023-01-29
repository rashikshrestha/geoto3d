const char UNDISTORT_FRAGMENT_SHADER[] = "						                \n\
#version 110                                                                    \n\
                                                                                \n\
uniform  vec3  kappa;                                                           \n\
uniform  float vignet;                                                      	\n\
uniform  float gamma;		                                                    \n\
uniform  float scale;		                                                    \n\
uniform  vec3  wb;			                                                    \n\
                                                                                \n\
varying	vec4 vPos;                                                              \n\
                                                                                \n\
uniform sampler2D tex;                                                          \n\
uniform mat4 matKtex;                                                           \n\
                                                                                \n\
void main(void)                                                                 \n\
{                                                                               \n\
    vec4  tex_color;															\n\
    vec3  out_color;															\n\
    																			\n\
	vec4 ray = vPos/vPos.z;                                                     \n\
	ray.w = 1.0;                                                                \n\
                                                                                \n\
	float r2 = ray.x*ray.x + ray.y*ray.y;                                       \n\
	float r4 = r2*r2;                                                           \n\
	float r6 = r4*r2;                                                           \n\
                                                                                \n\
	float fact  = 1.0+kappa.x*r2+kappa.y*r4+kappa.z*r6;                         \n\
	vec4 newPos = vec4(ray.x*fact,ray.y*fact,ray.zw);                           \n\
	vec4 tc		= matKtex*newPos;												\n\
	tc			= tc/tc.w;														\n\
                                                                                \n\
	float vig_cos   = 1.0/length(ray.xyz);                                     	\n\
    float vig_scale = 1.0/pow(vig_cos,vignet);                             		\n\
    tex_color	 	= texture2D(tex,tc.xy);										\n\
    out_color	 	= wb*tex_color.xyz;											\n\
    out_color	 	= vig_scale*out_color;  									\n\
    out_color	 	= pow(out_color , vec3(gamma));								\n\
    out_color	 	= min(scale*out_color,vec3(1.0));							\n\
    if ((tc.x>0.02) && (tc.x<0.98) && (tc.y>0.02) && (tc.y<0.98))				\n\
		gl_FragColor 	= vec4(out_color,1.0);									\n\
	else																		\n\
		gl_FragColor 	= vec4(out_color,0.0);									\n\
}                                                                               ";
