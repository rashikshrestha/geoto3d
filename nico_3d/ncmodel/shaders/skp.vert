#version 400

layout(location = 0) in vec3 mPosition;

void main(void)
{
	gl_Position = vec4(mPosition,1.0f);
}
