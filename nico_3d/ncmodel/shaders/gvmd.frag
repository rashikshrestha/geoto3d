#version 400
#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_gpu_shader5 : enable

layout(location = 0) out vec4    fColor;

void main(void) {

    fColor = vec4(gl_FragCoord.z);
}
