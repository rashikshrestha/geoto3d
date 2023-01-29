#version 330 core

layout(location = 0)    in		vec3 mPosition;

out float zPos;

void main(void)
{
    zPos        = mPosition.z;
	gl_Position = vec4(mPosition,1.0f);
}
