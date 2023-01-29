#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_gpu_shader5 : enable

uniform float *pbo3D;

uniform ivec3   size;

void main() {

    ivec2 tc = ivec2(gl_FragCoord.xy);

    for (int z=0;z<size.z;++z)
    {
        float error = getErrorPass1(Zlut[z]);
        pbo3D[(tc.y*size.x+tc.x)*size.z+z] = error;
    }

}

