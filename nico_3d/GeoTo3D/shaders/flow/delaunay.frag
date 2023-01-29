#version 330 core

in  float   zPos;

out vec2    fColor;

void main(void)
{
	fColor          = vec2(zPos,1.0f);
}
