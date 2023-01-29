#version 330

layout(location = 0) in vec3 mPosition;
layout(location = 1) in vec4 mColor;

uniform mat4 MVP;

out vec4 color;

void main(void)
{
    color       = mColor;
	gl_Position = MVP*vec4(mPosition,1.0f);

}
