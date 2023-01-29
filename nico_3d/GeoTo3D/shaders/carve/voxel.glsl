#version 400
#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_gpu_shader5 : enable

coherent volatile uniform int*  vPBO;

uniform int     minlevel;
uniform int     maxlevel;

uniform mat4    MVP;
uniform vec3    campos;

uniform sampler2D dTex;

