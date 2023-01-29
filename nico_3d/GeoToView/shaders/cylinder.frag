#version 330 core
#extension GL_EXT_gpu_shader4 : enable

uniform mat4	matP;
uniform	vec4	mColor;
uniform	float	mRadius;
uniform	int		model_ID;

in vec4 gPos;
in vec4 gBase;
in vec4 gTop;

layout(location = 0) out vec4 fColor;
layout(location = 1) out int  iColor;

void main(void)
{
	vec3	ray	= normalize(gPos.xyz);
	float	len = length(gTop.xyz-gBase.xyz);
	vec3	dir = (gTop.xyz-gBase.xyz)/len;
	float	RD	= dot(ray,dir);
	float	BD	= dot(gBase.xyz,dir);
	float	RB	= dot(ray,gBase.xyz);
	float	BB	= dot(gBase.xyz,gBase.xyz);
	float	A	= 1.0f - RD*RD;
	float	B	= 2.0f*(BD*RD-RB);
	float	C	= BB - BD*BD - mRadius*mRadius;
	float	D	= B*B - 4.0f*A*C;
	if (D< 0.0)
		discard;
	else
	{
		float t = (-B - sqrt(D))/(2.0f*A);
		vec3  I = t*ray;
		float K = dot(I-gBase.xyz,dir);
		if ((K>=0.0f) && (K<=len))
		{
			vec3  C = gBase.xyz + K*dir;
			vec3  N = normalize(I-C);
			float alpha = abs(dot(-ray,N));
			fColor = alpha*mColor;
			vec4 pp = matP*vec4(I,1.0f);
			gl_FragDepth = 0.5f*(pp.z/pp.w)+0.5f;
			iColor  = (model_ID<<24)|(gl_PrimitiveID & 0x00FFFFFF);
		}
		else
			discard;
	}
}	