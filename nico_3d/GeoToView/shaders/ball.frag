#version 330 core
#extension GL_EXT_gpu_shader4 : enable

uniform mat4	matP;
uniform	vec4	mColor;
uniform	float	mRadius;
uniform	int		model_ID;

in vec4 gPos;
in vec4 gCenter;

layout(location = 0) out vec4 fColor;
layout(location = 1) out int  iColor;

void main(void)
{
	vec3 ray	= normalize(gPos.xyz);
    vec3 dst	= gCenter.xyz;
	float B = dot(dst, ray);
	float C = dot(dst, dst) - mRadius*mRadius;
	float D = B*B - C;
	if (D< 0.0)
		discard;
	else
	{
		vec3 I	= (B - sqrt(D))*ray;
		vec3 N  = normalize(I-gCenter.xyz);
		fColor	= dot(N,-ray)*mColor;
		vec4 pp = matP*vec4(I,1.0f);
		gl_FragDepth = 0.5f*(pp.z/pp.w)+0.5f;
		iColor  = (model_ID<<24) | (gl_PrimitiveID & 0x00FFFFFF);
	}
}
