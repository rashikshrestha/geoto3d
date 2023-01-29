#version 330

in  vec4 color;
out vec4 fColor;

uniform float depth_offset;

void main(void)
{
    fColor = color;
}
