#extension GL_NV_gpu_shader5 : enable

layout(location = 0) out vec4    fColor;

uniform sampler2D   cTex;
uniform sampler2D   dTex;
uniform mat4        MVi;
uniform float       zfar;

void main(void) {

    vec4 p4;
    bool valid;
    processPixel(p4,valid);

    vec4 p2D = MVP*p4;
	float depth = 0.5f+0.5f*p2D.z/p2D.w;

    vec4 p3D = MVi*p4;
	p3D/=p3D.w;

	if (valid && (length(p3D.xyz)<zfar)) // && (depth < texelFetch(dTex,ivec2(gl_FragCoord.xy),0).x + 0.01f))
	{
        ivec4 p0 = ivec4(255.0f*texture2D(cTex,gl_FragCoord.xy/textureSize(cTex,0)));

        int fact = 1;//(int)(1.0f+8.0f*clamp(1.0f-depth,0.0f,1.0f));

        /*
        int tmp = voxels[vindex].carve&0xFF;
        tmp |= p0.x<<24;
        tmp |= p0.y<<16;
        tmp |= p0.z<<8;
        voxels[vindex].carve = tmp;
        */
        atomicAdd(color + 4*vindex + 0 , fact*p0.x);
        atomicAdd(color + 4*vindex + 1 , fact*p0.y);
        atomicAdd(color + 4*vindex + 2 , fact*p0.z);
        atomicAdd(color + 4*vindex + 3 , fact     );
	}
}
