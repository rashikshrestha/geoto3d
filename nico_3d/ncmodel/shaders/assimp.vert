#version 330

layout(location = 0) in vec3 mPosition;
layout(location = 1) in vec2 mTexCoord;
layout(location = 2) in vec4 mColor;

out vec2 tc_in;
out vec4 color_in;

void main(void)
{
	gl_Position	= vec4(mPosition,1.0f);
    tc_in			= mTexCoord;
    color_in		= mColor;
}
